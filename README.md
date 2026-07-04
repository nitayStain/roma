# roma

Roma (Runtime Oriented Machine Assembly, or just "roma") is a small
programming language and toolchain. `roma` is the master binary you run
to compile your source; it's a thin CLI driver linking together the
project's modules, each built as its own static library:

- `roc` - the roma compiler: lexer, parser, AST.
- `napoli` - the JIT module. Currently just a stub.

More modules will show up here as the project grows.

## Building

```sh
make
```

Builds `roc` (`roc/libroc.a`) and `napoli` (`napoli/libnapoli.a`), then
links them both into the top-level `roma` binary. Each module has its own
`include/`, `src/`, and `Makefile`, and can be built on its own via
`make -C roc` / `make -C napoli` (or `make roc` / `make napoli` from the
top level).

## Running

```sh
./roma path/to/file.ro
```

## Layout

```
roma/
  src/            the roma CLI driver (main.c), links roc + napoli
  roc/            lexer, parser, AST (static library)
    include/        public headers (lexer/ parser/ error.h logger.h ...)
    src/            implementation
  napoli/         JIT module (static library)
    include/napoli.h
    src/
```

## TODO

- [ ] napoli: actual JIT implementation (currently just a stub call from roc)
- [ ] roc: function declarations/calls in the language itself
- [ ] roc: a real type-checking / semantic analysis pass
- [ ] runtime errors (try/catch)
- [ ] figure out how the AST gets handed to napoli (direct, or through a bytecode IR)
