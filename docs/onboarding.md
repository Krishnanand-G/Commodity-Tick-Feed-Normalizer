# Content Type Onboarding

Adding a new content type to the parser requires zero changes to the core logic.

## Steps

1. **Identify the feed symbol.** E.g., `NG_FUT` for Natural Gas Futures.
2. **Determine the target normalized symbol.** E.g., `NG`.
3. **Register mapping.** In `Parser::Parser()`, add:
   ```cpp
   symbolLookup["NG_FUT"] = "NG";
   ```
4. **Compile and test.** Run the test suite to ensure no regressions occurred.

The parser will now automatically recognize `NG_FUT` strings in the raw CSV, assign the sequence ID and timestamp appropriately, and emit `Quote` structs tagged with `NG`.
