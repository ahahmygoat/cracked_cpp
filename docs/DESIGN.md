# Design and OOP: Think like a principal engineer (and a PM)

This doc explains **why we structure code the way we do**: OOP ideas, design tradeoffs, when to refactor, and when to think like a PM. No prior design theory assumed — we use plain language and tie everything to this repo.

**Software-engineering concepts:** For **encapsulation**, **inheritance**, and **polymorphism** (with examples and when to use them), see [oop-concepts.md](oop-concepts.md).

---

## 1. Why structure code at all?

**Noob takeaway:** Code that "just works" today becomes hard to change tomorrow. Structure (functions, classes, files) makes it easier to find things, change one part without breaking others, and onboard people.

**Principal takeaway:** Good structure encodes *intent* and *invariants*. If "one function does one job" and "one class owns one kind of data," the system is easier to reason about, test, and evolve. Bad structure hides bugs and makes every change risky.

**PM lens:** If product says "add feature X," you want to know *where* X lives and *what* it might break. Clear structure answers that. If no one can point to "the place that does Y," refactoring is a product investment, not just tech debt.

---

## 2. Single responsibility (one job per thing)

**Idea:** A function or class should have **one reason to change**. If it does two unrelated things, changing one requirement forces you to touch the same code as the other — more risk, harder reviews.

**In this repo:**
- **main.cpp** — one big `main()` does menu, input, validation, and actions. Simple to read in one place, but "one change" (e.g. validation rules) lives next to menu text and business logic.
- **refactorMain.cpp** — `printMenu()`, `getUserOption()`, `validateUserOption()`, `handleUserOption()` each do one job. Changing validation doesn’t touch menu text; changing the menu doesn’t touch validation.

**When to split:** When you find yourself editing the same block for two different reasons (e.g. "fix validation" and "add a new menu item"), that block has more than one responsibility. Split by *reason to change*.

**PM lens:** "We need to change how we validate input" — with single responsibility, that’s one function. Without it, it’s "search the whole file." Refactoring toward single responsibility is a product enabler: faster, safer changes.

---

## 3. Data vs behavior: what belongs in a class?

**Idea:** A **class** bundles **data** (what it knows) and **behavior** (what it does). The data should be the minimum needed to implement the behavior; the behavior should be the operations that make sense on that data.

**In this repo:**
- **OrderBookEntry** — data: price, amount, timestamp, product, orderType. Behavior: `print()`. We could add `isValid()` or `toCsvLine()` later; they belong here because they depend on this data.
- **Wallet** (when we add it) — data: balance, maybe currency. Behavior: deposit, withdraw, getBalance. The class *owns* the invariant "balance ≥ 0" (if we enforce it).

**Principal takeaway:** If a function only needs one or two fields from a class, maybe it shouldn’t be a member — it could be a free function. If a function touches many fields and invariants, it belongs in the class. "Does this operation need to know the whole object?" → often a good hint.

**Noob takeaway:** Start with "this type holds this data." Then ask: "what do we always do with this data?" Those actions become member functions. Don’t put random functions in a class just because they mention the type.

---

## 4. Public vs private: simplicity now, invariants later

**Idea:** **Public** data = anyone can read/write it. Simple, but you can’t enforce rules (e.g. "price must be positive"). **Private** data + getters/setters = you control how data changes, so you can enforce invariants.

**In this repo:** We use **public** data for `Order` and `OrderBookEntry` so you can see and set fields directly. The comments say "later: private + getters/setters if you need invariants." That’s intentional: learn the shape first, tighten when requirements demand it (e.g. "price can’t be negative").

**When to go private:** When you have a rule that must always hold (e.g. balance ≥ 0, or "timestamp must be parseable"). If you leave data public, someone will bypass the rule. Private + a single setter lets you check once.

**PM lens:** "We need to guarantee X" (e.g. no negative prices) → that’s when you invest in private data and validation in one place. Before that, public is fine; after that, it’s a product requirement, not just style.

---

## 5. When to refactor

**Principal takeaway:** Refactor when the *cost of not refactoring* (bugs, slow feature work, confusion) exceeds the *cost of refactoring* (time, risk). Don’t refactor "because it’s messy" alone; refactor when the mess is blocking you or will block the next person.

**Concrete triggers:**
- **Adding a feature** would require duplicating a big block of code → extract a function or class.
- **Changing one behavior** forces you to edit many places → that behavior should live in one place (single responsibility).
- **No one can explain** "where does X happen?" → add a named function or type so the map is clear.
- **Tests are hard** because you can’t call one piece without the whole program → split so you can test units.

**PM lens:** Refactoring is justified when it *unblocks* product work or reduces risk for the next release. "We need to add feature X and the current structure makes it dangerous" is a good reason. "We’ll clean it up someday" without a concrete need often never happens.

---

## 6. Tradeoffs we make (and own)

| Choice | Why we do it | Tradeoff / when to revisit |
|--------|--------------|----------------------------|
| One file, one `main()` (main.cpp) | Easiest to read: one flow. | Refactor when the file gets long or you need to test parts separately. |
| Functions per action (refactorMain) | One job per function; main() orchestrates. | Slightly more indirection; worth it for clarity and testing. |
| `enum class` for menu/order type | Type-safe; no magic numbers. | Slightly more verbose than `const int` or string; we accept that. |
| Public data for Order / OrderBookEntry | Simplest for learning and quick iteration. | When you need invariants (e.g. price ≥ 0), switch to private + getters/setters. |
| Global `orders` vector in OrderBookEntry.cpp | Simple: one place that holds loaded data. | When you have multiple books or need to pass the book around, inject it (e.g. pass vector or a "book" object). |
| Skip bad CSV lines (try/catch stod) | Don’t crash on one bad row; keep loading. | If product needs "report bad lines," we’d add logging or a separate validation pass. See [exception-handling.md](exception-handling.md). |

**Noob takeaway:** Every design is a tradeoff. We document *why* we chose something and *when* we’d change it so you see that there’s no single "right" answer — only answers that fit the current goals.

---

## 7. System thinking: how the pieces fit

**Idea:** The **system** is: input (user, file) → processing (validation, business logic) → output (screen, file, next system). Each piece should have a clear role so you can change one without breaking another.

**In this repo:**
- **Input:** keyboard (cin), CSV file (ifstream). We keep "read one line" and "parse one line" separate from "what we do with the data."
- **Processing:** menu choice → validate → handle. Validation doesn’t print the menu; handling doesn’t read raw input. That separation is system thinking: boundaries between layers.
- **Output:** cout, or (later) writing results back to file. If we add logging, it’s another output; we don’t mix it with "compute the next state."

**Principal takeaway:** Draw a tiny mental picture: where does data come in? where is it transformed? where does it go? Keep those boundaries clear. When "one module" does I/O and business logic and logging, the system is hard to test and change.

**PM lens:** "Where do we add feature X?" — answer in terms of layer: "X is new input" vs "X is new processing" vs "X is new output." That tells you which files and which tests to touch.

---

## 8. OOP in one page (for this repo)

- **Class** = type you define: what data it has (members) and what it does (member functions). Blueprint.
- **Object** = one instance of that type. Actual data in memory.
- **Encapsulation** = bundle data + behavior; hide data (private) when you need to enforce rules.
- **Single responsibility** = one reason to change per function/class.
- **Don’t over-OOP** = not everything needs to be a class. Free functions (e.g. `orderBookTypeToString`) are fine when they don’t need to own state.

We use OOP where it helps: one row of data → one type (`OrderBookEntry`). One collection of rows + operations → later, an `OrderBook` class. We don’t force every function into a class.

---

## 9. When to escalate (principal / PM)

- **"This will take long because the code is tangled"** → Propose a small refactor (extract function/class) so the feature has a clear home. Estimate refactor + feature; don’t hide refactor inside "feature estimate."
- **"Product wants a guarantee (e.g. no negative prices)"** → That’s the moment for private data + validation in one place. Call it out as a design change, not just a line of code.
- **"We’re duplicating the same logic in three places"** → Extract once, call everywhere. Reduces bugs and future change cost.
- **"New person can’t find where X happens"** → Add a named function or doc pointer. Onboarding cost is product cost.

Use this doc as a shared language: "we’re refactoring for single responsibility" or "we’re adding a private setter to enforce an invariant" so everyone (eng and PM) knows what’s being done and why.

---

## Related docs

- [INDEX.md](INDEX.md) — Learning path and doc map.
- [oop-concepts.md](oop-concepts.md) — Encapsulation, inheritance, polymorphism (with examples).
- [ORDERBOOK.md](ORDERBOOK.md) — Domain: order book, CSV.
- [organizing-code.md](organizing-code.md) — Limiting exposure, embedding init.
