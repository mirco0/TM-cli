# Turing Machine Manual

## Step 0 — Default values and reserved names

- `%` starts a comment; anything after `%` on a line is ignored.
- `\square` is the **blank symbol** on the tape.
- The machine always starts in state `"q0"` unless otherwise specified.
- The machine halts when it reaches state `"qF"` or any state in the set `QF`.
- The alphabet variable `\Sigma` defaults to the set `{ "0", "1" }` if not explicitly redefined.
- States, symbols, and other names are **case-sensitive strings**.
- You can define additional final states in the set `QF` (e.g., `QF = \{qAccept, qReject\}`).
- The machine halts when:
  - It reaches state `"qF"` or any state in `QF`.
  - No instruction applies to the current state and tape symbol.

### Termination

- The machine will terminate when it reaches the state `"qF"` or when it can't execute any instruction.
- It is possible to define a set `QF` to specify additional final states; the machine will terminate automatically when it reaches any of these states.

---

## Step 1 — Defining sets

You can define any number of **immutable sets** using this syntax:

```tm
A = \{ A, B, cd \}
C = \{ 0, 1 \}
```

This creates:
  - A set variable A containing the strings "A", "B", and "cd".
  - A set variable C containing the strings "0" and "1".

### Notes:
- Values inside curly braces \{ \} are always treated as literal strings, not as variables.

- Sets are **immutable** — once defined, they cannot be changed during runtime.

## Step 2 — Working with set variables

After declaring a variable it will be available for the whole life of the program and it can be used as follows:

- Assigning variables:
  ```tm
  D = A
  E = A \cup \{ F, M \}
  Foo = A \cap \{ B \}
  ```
- Using sets in instruction quantifiers:
  ```tm
  <q3,x,x,q4,Stop> \forall x \in A
  ```
  will be interpreted as:
  ```
  <q3,A,A,q4,Stop>
  <q3,B,B,q4,Stop>
  <q3,cd,cd,q4,Stop>
  
### Step 3 — Working with instance variables

Instance variables act as placeholders in instructions to simplify writing multiple transitions.

Example:
```tm
<q3,x,x,q4,Stop> \forall x \in A
```
  - You can use multiple variables for a single set to obtain the Cartesian product:
    ```
    <x,y,y,x,Right> \forall x,y \in \{q0,q1\}
    ```
    which will be interpreted as:
    ```
    <q0,q0,q0,q0,Right>
    <q0,q1,q1,q0,Right>
    <q1,q0,q0,q1,Right>
    <q1,q1,q1,q1,Right>
    ```
    where x and y assume all possible pairs of values.
  
  ### Important limitations 

  - You cannot **yet** quantify multiple variables over different sets in the same instruction (e.g., \forall v \in \{1,2\}, s \in \{0,1\} is not allowed).

  - Variables must be used literally; you cannot concatenate variables to form state or symbol names (e.g., qv is invalid; use v only).

## How to Write a Program
A Turing Machine program consists of assignments and instructions written line-by-line.

1) Define sets (optional but recommended)
    ```
    States = \{q0, q1, q2\}
    Symbols = \{0, 1, X, Y\}
    ```

2) Instructions have the form:
    ```tm
    <current_state, read_symbol, write_symbol, next_state, move_direction>
    ```
    every field except `move_direction` can be expressed with a instance variable and must be either
    `Right`,`Left`,`Stop`

    example:
    ```tm
    <q0,0,X,q1,Right>
    ```
    This means:

    If in state *q0* reading symbol *0*, write *X*, move *right*, and go to state *q1*.