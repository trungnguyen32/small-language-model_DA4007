# Small Language Model (SLM) 

## Algorithm

The model reads a plain ASCII training file. It counts every overlapping k-gram, including the last one, to obtain starting-context weights. Separately, it counts every `(context, next character)` pair for positions with a following character. The alphabet is the set of distinct input characters.

The next-character distribution is `P(c | w) = (n(w,c) + alpha) / (n(w) + alpha * alphabet_size)`. The implementation samples using these unnormalized weights, which is mathematically equivalent. When `alpha=0`, generation stops if the current context has no outgoing transition; when `alpha>0`, an unseen context is sampled uniformly over the alphabet.

## Build

```sh
make
```

## Run

```sh
./slm 2 training.txt 100
./slm -a 0.01 3 training.txt 100
```

The output length includes the starting k-gram. Random output varies between runs. A final newline is printed after the generated text.

## Test

```sh
bash tests/test_slm.sh

## Files

- `src/LanguageModel.*`: reading the training text, counting k-grams and transitions, and sampling characters.
- `src/TextGenerator.*`: generating text and updating the context.
- `src/main.cpp`: command-line validation and exception handling.
- `tests/test_slm.sh`:  tests and error-handling checks.

## Design choices and limitations

- Training files are assumed to be ASCII, as specified by the assignment.
- Input is loaded in memory for simplicity.
- `std::map` stores frequency tables; `std::discrete_distribution` handles weighted random sampling.
- There is no fixed RNG seed, so output is not deterministic.
- The test suite checks key cases but is not exhaustive.
