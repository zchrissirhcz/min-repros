# test_asan_for_intrinsics

When loading data from memory to NEON vector registers, can Address Sanitizer detect invalid memory read?

In this example, it invokes ARM NEON Intrinsics `vld1_u8` which will load from invalid memory.

To verify, a simulated `vld1_u8` is implemented for non-NEON case, such as Linux x64 compilation. The actual result:

| platform | ASan report invalid read ? |
| -------- | -------------------------- |
| arm32    | No |
| arm64    | No |
| linux-x64 | Yes |