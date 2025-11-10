# Interactive mode

The interactive mode can be used to play the hash table through a console-like interface.
The user can interact with a hash table by running actions line-by-line.

### Available commands:
- `add {key} {value}`: adds a key-value pair to the table
- `get {key}`: get a value associated to the key
- `del {key}`: deletes an entry
- `save`: saves the current table
- `load`: loads the last saved table
- `print`: prints the table
- `exit`: exits interactive mode

### Example usage

```
CHashTable Interactive Mode
---------------------------
Available commands:
> add {key} {value}
> get {key}
> del {key}
> save
> load
> print
---------------------------

> add 1 10
> add 2 20
> get 1
10
> get 7
NULL
> del 2
> get 2
NULL
> save
> del 1
> get 1
NULL
> load
> get 1
10
> exit
```