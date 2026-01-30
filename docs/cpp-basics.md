# C++ basics: functions, variables, control flow

Elementary concepts as used in **main.cpp** and **refactorMain.cpp**. No prior C++ assumed.

**How we document:** We explain _why_ something exists and what happens if you change it, not just _what_ it does. Design choices are called out so you can see the tradeoffs. If you're new, read the "What is…" and "Why…" bits; the code comments in the repo mirror this style.

**Principal takeaway:** Functions are the smallest unit of "one job." `main()` is the single entry point the OS calls; everything else is structure we choose. By value vs by reference is a design choice: "does the caller need to see changes?" → reference; "do we only read it?" → value. See [DESIGN.md](DESIGN.md) for single responsibility and when to split.

---


## 1. Functions (and why you need `main`)

### What is a function?

A **function** is a named block of code you can run by calling it. It can take inputs (parameters) and optionally return one value.

- **Definition**: you write the function once (name, parameters, body in `{ }`).
- **Call**: you run it by writing its name followed by `( )` (and arguments if it has parameters).

### Every program must have `main()`

The OS runs your program by calling **`main()`**. So:

- You **must** define exactly one `main()` in your program.
- Execution **starts** at the first line inside `main()`.
- When `main()` returns (e.g. `return 0;`), the program exits.

```cpp
int main() {
    // code here runs first
    return 0;   // 0 = success
}
```

If you have no `main()`, the linker will report an error (e.g. "undefined reference to main").

### Return type: `void` vs a type (e.g. `int`)

- **`void`** = this function does **not** return a value. You just call it and it does something (e.g. print the menu).
  - Example: `void printMenu() { ... }`
- **`int`** (or another type) = this function **returns** one value of that type. You use `return value;` and the caller can store or use it.
  - Example: `int getUserOption() { ... return userOption; }`

### Parameters: inputs to the function

- **By value** (`int x`): the function gets a **copy**. Changing `x` inside the function does not change the caller's variable.
  - Example: `void handleUserOption(MenuOption choice)` — we only read `choice`.
- **By reference** (`int& x`): the function gets the **actual** variable. Changing `x` inside the function changes the caller's variable.
  - Example: `void validateUserOption(int& userOption)` — we need to update the caller's `userOption` when they re-enter.

### Where you see this in our code

- **main.cpp**: one big `main()` with menu, input, validation, and if/else if for each option.
- **refactorMain.cpp**: `main()` is short; it calls `printMenu()`, `getUserOption()`, `validateUserOption()`, `handleUserOption()`. Each of those is a separate function. The program still **has** to have `main()` so the OS knows where to start.

---

## 2. Variables

### Declaration and types

A **variable** holds a value. You **declare** it with a type and a name:

```cpp
int userOption;      // whole number
int amount, price;   // two ints
```

- **`int`** = integer (whole number). Used in both files for menu choice, amount, price.
- Other types you'll see later: `double` (decimal), `std::string` (text), etc.

### Assignment and input

- **Assign**: `userOption = 5;`
- **Read from keyboard**: `std::cin >> userOption;` — whatever number they type gets stored in `userOption`.

### Scope (where a variable exists)

Variables live only inside the **block** (the `{ }`) where they are declared.

- A variable declared inside `main()` is only visible inside `main()`.
- A variable declared inside an `if` or `else` block is only visible in that block. So in `main.cpp` we can use `int amount;` in both the "offer" and "bid" branches — they are different variables.

### Constants

- **`const int OPTION_HELP = 1;`** — the value does not change. Good for fixed menu options so you don't scatter "magic numbers" (1, 2, 3...) everywhere.
- **`const char SEP[] = "..."`** — constant string used as the separator line in the menu.

---

## 3. Control flow

Control flow = **which code runs next**: conditionals, loops, and branches.

### `if` / `else if` / `else`

Run **at most one** of the blocks. The first condition that is true wins; the rest are skipped.

```cpp
if (userOption == 1) {
    // do help
} else if (userOption == 2) {
    // do stats
} else if (userOption == 3) {
    // make offer
}
// ...
```

- **`==`** means "equal to" (comparison). **`=`** alone is assignment.

### `while (condition) { ... }`

Repeat the block **while** the condition is true. When the condition becomes false, execution continues after the loop.

- In **main.cpp** and **refactorMain.cpp**: we use a `while` loop to re-prompt until the user enters a number between 1 and 6.
- **`while (true)`** in `main()` runs forever until we hit `break` (e.g. when they choose "Continue").

### `switch (value) { case ... break; }`

Pick **one** branch based on the value. Cleaner than a long chain of `else if` when you're comparing one variable to many fixed values.

- In **refactorMain.cpp**: `switch (choice)` with `case MenuOption::Help:`, `case MenuOption::Stats:`, etc. Each `break;` exits the switch so only one case runs.

### `break` and `return`

- **`break`** — exit the innermost `switch` or loop (`while`, `for`). In our main loop, `break` exits the `while (true)` when the user picks "Continue".
- **`return value;`** — exit the current function and send `value` back to the caller. **`return;`** (in a `void` function) — just exit the function.

---

## Quick reference (as in main.cpp / refactorMain.cpp)

| Concept                 | Example / meaning                                                          |
| ----------------------- | -------------------------------------------------------------------------- |
| Must have main          | `int main() { ... return 0; }` — program starts here.                      |
| Function, no return     | `void printMenu() { ... }`                                                 |
| Function, returns value | `int getUserOption() { ... return userOption; }`                           |
| Parameter by value      | `void handleUserOption(MenuOption choice)` — copy, read-only.              |
| Parameter by reference  | `void validateUserOption(int& userOption)` — can change caller's variable. |
| Variable                | `int userOption;` then `std::cin >> userOption;`                           |
| Constant                | `const int OPTION_HELP = 1;`                                               |
| If / else if            | One block runs; first matching condition wins.                             |
| While loop              | Repeat while condition true; re-prompt until valid input.                  |
| Switch                  | Multi-way branch: `switch (choice) { case X: ... break; }`                 |
| Break                   | Exit current loop or switch.                                               |
| Return                  | Exit function; optionally send a value back.                               |

---

For full project setup and running, see [SETUP.md](SETUP.md). For Git/GitHub, see [git-github-cheatsheet.md](git-github-cheatsheet.md). For OOP and design (principal/PM level), see [DESIGN.md](DESIGN.md). For the full doc map, see [INDEX.md](INDEX.md).
