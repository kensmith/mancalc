// This exists to make sure libtestables.a has something in
// it in the event all library code is header-only. The
// alternative would be to create libtestables.a only if
// there were some .cpp library files. It's easier to
// unconditionally create the library with a useless symbol
// which is elided from the link anyway.
void foo() {}
