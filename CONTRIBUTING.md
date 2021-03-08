# Guidelines for Contributors

## Coding Conventions

This code base will have a pre-made `.clang-format` to take care of most formatting
issues automatically. Use `snake_case` for function and variable names, and
`CamelCase` for classes, structs, and so on.

## Branching Policy

Create a new topic branch based of `integration`, and raise PRs against this branch
back once you're done with your work. Your code will be reviewed before a merge can
be confirmed. After a while, `integration` merges into `master`, marking a new
and thoroughly tested release.

```bash
master
  |
  \ ____ integration
             |
             |\ ____ pokemon
             |
             |\ ____ ui
             |
             \ _____ api
```
