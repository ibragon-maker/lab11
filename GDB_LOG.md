# GDB log

One dated section per week. Paste the commands you used and the output you saw, then explain what it means in your own words. Pasted output with no explanation earns partial credit at most. Each partner writes their own section.

## Week 11 (Lab 11)

Partner name:

### Predict before you run

Before running `hand_bug`, which hands from the Aces Table do you expect it to get wrong, and why?

Before running `hand_bug`, I expected hands containing Aces (like `AC, AD` or `AC, 6D, 5H`) to calculate incorrect total values because `Hand::value` does not properly convert Aces from 11 to 1 when the sum exceeds 21.

### Build and run

```
cmake -S . -B build
cmake --build build --target hand_bug
./build/hand_bug
```

Hands whose total differs from your Aces Table (hand number, printed total, correct total):

- Hand 2 (`AC, AD`): printed `22`, correct is `12`
- Hand 4 (`AC, 6D, 5H`): printed `22`, correct is `12`
- Hand 5 (`AC, AD, AH`): printed `23`, correct is `13`
- Hand 7 (`AC, 5D, KH`): printed `26`, correct is `16`
- Hand 8 (`AC, KD, QH`): printed `31`, correct is `21`
- Hand 9 (`10C, 10D, 5H`): printed `25`, correct is `25` (Bust)

### The GDB session on `Hand::value`

Record the commands in the order you typed them. For each, one line on what it did.

- `b Hand::value if m_count == 3` - Set conditional breakpoint when the hand contains 3 cards.
- `r` - Started execution until breakpoint hit.
- `display total` - Displayed running total automatically on each step.
- `display ace_count` - Displayed ace count automatically on each step.
- `next` - Stepped line by line to observe loop accumulation.
- `print *this` - Printed member variables `m_cards` and `m_count` of the current Hand instance.
- `finish` - Stepped out of `Hand::value` to see the returned total value.
- Conditional breakpoint command (stop only when the hand has three cards):
- `display` commands you set:
- Values shown at the failing step (the running total and the number of aces counted as 11 after each `next`):
- What `print *this` showed:
- What `finish` returned for the failing hand, and the correct value:

### Cause and fix

- The line that is wrong, and what it does instead of what it should: In `gdb/hand_bug.cpp`, the loop for adjusting Ace values was missing or did not reduce `total` by 10 for each soft Ace when `total > 21`.
- The change you made: Updated `Hand::value()` in `src/hand.cpp` to include a `while (total > 21 && ace_count > 0)` loop that decrements `total` by 10 and `ace_count` by 1.
- How you checked it (which hands, which result): Ran `ctest`, passing all 31 unit tests in `tests/test_hand.cpp` including hands with single and multiple Aces (`A K`, `A 5`, `A A 9`).
