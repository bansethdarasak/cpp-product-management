# Product Management App

# How to Run the Project

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

After building successfully, run the application:

```bash
./ProductManagementApp
```

---

# Errors relating to (xlnt Library)

If you encounter compilation errors related to:

- `uint32_t`
- `uint64_t`
- `int32_t`

You need to add:

```cpp
#include <cstdint>
```

to the following header files inside the generated `build` directory.

## Files to Edit

### 1. `build/_deps/xlnt-src/source/detail/serialization/phonetic_run.hpp`

### 2. `build/_deps/xlnt-src/source/utils/time.hpp`

### 3. `build/_deps/xlnt-src/source/utils/timedelta.hpp`

### 4. `build/_deps/xlnt-src/source/utils/variant.hpp`

---

# Rebuild the Project

After adding the missing include, rebuild the project:

```bash
cmake --build .
```

Then run the application:

```bash
./ProductManagementApp
```