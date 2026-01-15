#include <assert.h>
#include <dynload.h>
#include <uv.h>

int
main() {
  int e;

  char path[4096];
  size_t len = sizeof(path);
  e = dynload_resolve("foo", "test/fixtures/lib", path, &len);
  assert(e == 0);

  printf("path=%s\n", path);
}
