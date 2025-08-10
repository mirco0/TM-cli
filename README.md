# TM-CLI: A Turing Machine Language & Parser

TM-CLI is a domain-specific language and interpreter for defining and executing Turing Machines.

It allows set definition, quantification and transition rules to make Turing Machine programs more expressive.

This project aims to parse and execute code written for Turing Machines with a sprinkle of math syntax 

$$ 
\begin{align*}
& \Sigma = \\{1,2,3,4\\}\\
& A = \\{2\\}\\
& \langle q_0,x,x,q_1,Right \rangle \forall x \in \Sigma - A
\end{align*}
$$

## How to use
You can find a guide on how to use the TM-CLI[here](HOW_TO.md)
## Features
The parser is almost complete, but some features are still missing:

- [x] **Set operations** (`\cup`, `\cap`, `-`) for defining input alphabets
- [x] **Symbolic sets** like `\Sigma` and named subsets
- [x] **Quantified instructions** with `\forall x \in ...`
- [x] **Automatic instruction expansion** from symbolic domains
- [x] **Comment support** via `%`
- [x] Comment support at end-of-line (inline)

- [x] Turing Machine execution engine (work in progress)

- [x] Terminal visualization of tape and current state (work in progress)

- [ ] Step-by-step execution mode with debug output

- [ ] Better error reporting and diagnostics


## Formal Grammar
The formal grammar for this project at the moment is:

```
program           → statement
statement         → (assignment | instruction)*

instruction       → "<" ID (","ID){3} "," ("Left" | "Stop" | "Right") ">" (domain)?
assignment        → (IDENTIFIER | \Sigma) "=" set_op
domain            → "\forall" variables "\in" set_op

variables         → IDENTIFIER ("," IDENTIFIER)*  
set_op            → set_difference
set_difference    → set_union ( - set_union)* 
set_union         → set_intersection ( "\cup" set_intersection)*
set_intersection  → set_elements ( "\cap" set_elements )*
set_elements      → ( "\{" (IDENTIFIER | "\square") ( "," (IDENTIFIER | "\square") )* "\}" ) | SET
SET               → \Sigma | IDENTIFIER

```

## Example Program
[Program.tm](Program.tm)
```tm
% Input alphabet 
\Sigma = \{1,2,3,4\}
A = \{2\}

% Instruction using quantified domain
<q0, x, x, q1, Right> \forall x \in \Sigma - A
```
Gets expanded to

```tm
<q0,1,1,q1,Right>
<q0,3,3,q1,Right>
<q0,4,4,q1,Right>
```
## Notes
This project is highly inspired by the course "Fondamenti di informatica" attended at Università di TorVergata and references formal language design and interpreters 
[Crafting Interpreters](https://craftinginterpreters.com/contents.html)

### Resources
[Crafting Interpreters](https://craftinginterpreters.com/contents.html)

[HashTables in C](https://benhoyt.com/writings/hash-table-in-c/)

This project is a far from completion and primarily built for learning and experimentation.

## License
Licensed under [MIT LICENSE](LICENSE)
