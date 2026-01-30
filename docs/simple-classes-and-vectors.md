# Simple classes and putting objects into vectors

This doc covers **syntax for working with simple classes** and **storing objects in a `std::vector`**: from procedural code and global data to encapsulating related data in a class, then putting many instances into a growable container. It mirrors the “classes and data” lab: why we use classes, how to define one, and how to use a vector of objects.

**Takeaway:** A class is a **new data type** that groups data (and later, behavior) that belongs together. You can then store many instances of that type in a `std::vector<T>` and access them with the usual indexing and dot syntax.

---

## 1. From procedural to classes

### Procedural style: functions + global data

In a **procedural** style you have a set of functions and **global data** that any function can read or change:

```cpp
int x1, x2;  // global — visible to everything

void doSomething() {
    // can read/write x1, x2
}
void doSomethingElse() {
    // can read/write x1, x2
}

int main() {
    x1 = 1;
    x2 = 2;
    doSomething();
    doSomethingElse();
    return 0;
}
```

That works, but it doesn’t give a clear way to represent **data that belongs together**: e.g. one order book entry (price, amount, timestamp, product, bid/ask), or one student (name, id, grade), or one 3D vector (x, y, z). Without classes you end up with many separate variables (`x1, y1, z1`, `x2, y2, z2`, …) and no explicit grouping.

### Why encapsulate?

**Encapsulation** means grouping related data (and later, the operations on that data) into a single type so that:

- It’s obvious which pieces of data form one “thing.”
- You can work with one variable per logical entity (e.g. one 3D vector) instead of three separate variables.

A **class** in C++ is exactly that: a way to define a new data type and say “these members stick together.”

---

## 2. Class = new data type (simple example: Vec3D)

A class is a **new data type**, like `int` or `double`. You define it with the keyword **`class`**, a name, and a body ending with a **semicolon**.

### Minimal class with public data

```cpp
class Vec3D {
public:
    double x;
    double y;
    double z;
};   // ← semicolon required
```

- **`public:`** — everything below it is visible from outside the class (e.g. from `main`).
- **Members** `x`, `y`, `z` — three `double`s that “belong together” as one 3D vector.

### Using the class: one variable, dot access

You declare a variable of that type and use the **dot (`.`)** to get at its members:

```cpp
int main() {
    Vec3D vec1;           // one variable representing one 3D vector
    vec1.x = 1.0;
    vec1.y = 2.0;
    vec1.z = 3.0;

    Vec3D vec2;
    vec2.x = 4.0;
    vec2.y = 5.0;
    vec2.z = 6.0;

    // use vec1, vec2 as single units
    return 0;
}
```

So instead of six separate variables (`x1, y1, z1, x2, y2, z2`), you have two variables (`vec1`, `vec2`) and you “dig into” them with `.x`, `.y`, `.z`. The variable name is like a small namespace for its members.

---

## 3. Public vs private

You can hide implementation details by marking members as **`private`**. Only code inside the class (and friends, not discussed here) can access private members; the compiler will report an error if you try from outside.

### Making members private

```cpp
class Vec3D {
private:
    double x;
    double y;
    double z;
};
```

In `main` (or any non-member code):

```cpp
vec1.x = 1.0;   // ERROR: 'x' is a private member of 'Vec3D'
```

The compiler message is along the lines of: **“that is a private member of Vec3D, so you can’t access that.”**

Later you’ll expose only what’s needed via **public** member functions (e.g. `getX()`, `setX()`), and keep data and internal helpers **private**. That way the class behaves like a black box: the rest of the program uses only the public interface.

---

## 4. Putting objects into a vector

You often want **many** instances of your type (e.g. many order book rows, many 3D points). You can store them in a **`std::vector<T>`**, where `T` is your class.

### Declare a vector of your type

```cpp
#include <vector>

std::vector<Vec3D> coords;   // "vector" of Vec3D — growable array of Vec3D objects
```

- **`std::vector<Vec3D>`** — a dynamic array whose elements are `Vec3D` objects.
- **`coords`** — the variable; initially empty (no elements).

### Add objects with push_back

```cpp
Vec3D vec1;
vec1.x = 1.0;
vec1.y = 2.0;
vec1.z = 3.0;

coords.push_back(vec1);   // append vec1 (a copy) to the end of coords
```

You can push more `Vec3D` values; the vector grows as needed.

### Access with index and dot

Use the usual **index** `[i]` to get the `i`-th element; then use the **dot** to access its members:

```cpp
coords[0].x = 10.0;           // first element's x
double y0 = coords[0].y;      // first element's y
```

So you have:

- **Basic types** (e.g. `int`, `double`) →
- **Wrapped in a class** (e.g. `Vec3D` with `x`, `y`, `z`) →
- **Many instances in a container** (`std::vector<Vec3D>`).

That’s the layering: types → class → vector of objects.

---

## 5. Quick reference: syntax

| Idea | Syntax |
|------|--------|
| Define a class | `class Name { public: ... };` (semicolon after `}`) |
| Public / private | `public:` and `private:` sections; only public is visible outside |
| Use an object | `VariableName.memberName` (e.g. `vec1.x`) |
| Vector of your type | `std::vector<ClassName> name;` |
| Add to vector | `name.push_back(object);` |
| Access i-th element | `name[i]` → then `name[i].memberName` |

---

## 6. Where this appears in the repo

- **src/OrderBookEntry.cpp** — Defines class `OrderBookEntry` (price, amount, timestamp, product, orderType). CSV loading is in **src/CSVReader.cpp**; OrderBookEntry demo fills a vector and prints the first N entries. Same idea: simple class + vector of objects.

---

## Related docs

- [vector-iteration.md](vector-iteration.md) — Iterating over the vector (index, iterator, range-for).
- [ClassesandData.md](ClassesandData.md) — Types, struct vs class, vectors, error handling.
- [project-layout.md](project-layout.md) — Where OrderBookEntry and CSVReader live (src/).
