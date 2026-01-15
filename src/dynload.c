#include <assert.h>
#include <path.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>

#include "../include/dynload.h"

int
dynload_parse(const char *file, char *name, size_t len, dynload_version_t version) {
  int err;

  if (strncmp(file, "lib", 3) == 0) file += 3;

  int end = 0;

  while (file[end] != '.' && file[end] != '-' && file[end] != '\0') {
    end++;
  }

  if (file[end] != '.' && file[end] != '-') return -1;

  if (len - 1 < end) return -1;

  memcpy(name, file, end);

  name[end] = '\0';

  file += end + 1;

  for (int i = 0; i < 3; i++) {
    int n;
    err = sscanf(file, "%d.%n", &version[i], &n);
    if (err != 1) return -1;

    file += n;
  }

  if (strncmp(file, "so", 2) == 0) file += 2;
  else if (strncmp(file, "dylib", 5) == 0) file += 5;
  else if (strncmp(file, "dll", 3) == 0) file += 3;
  else return -1;

  if (file[0] != '\0') return -1;

  return 0;
}

int
dynload_compare(dynload_version_t a, dynload_version_t b) {
  for (int i = 0; i < 3; i++) {
    if (a[i] < b[i]) return -1;
    if (a[i] > b[i]) return 1;
  }

  return 0;
}

int
dynload_resolve(const char *name, const char *prefix, char *result, size_t *len) {
  int err;

  uv_fs_t fs;
  err = uv_fs_opendir(NULL, &fs, prefix, NULL);
  if (err < 0) return err;

  uv_dir_t *dir = fs.ptr;

  uv_dirent_t entries[64];

  dir->dirents = entries;
  dir->nentries = 64;

  char path[4096];
  bool found = false;
  dynload_version_t version;

  while (true) {
    err = uv_fs_readdir(NULL, &fs, dir, NULL);
    if (err < 0) goto err;

    if (err == 0) break;

    for (int i = 0, n = err; i < n; i++) {
      uv_dirent_t *entry = &entries[i];

      char candidate_name[256];
      dynload_version_t candidate_version;

      err = dynload_parse(entry->name, candidate_name, sizeof(candidate_name), candidate_version);
      if (err < 0) continue;

      if (strcmp(name, candidate_name) != 0) continue;

      if (found && dynload_compare(version, candidate_version) > 0) continue;

      found = true;

      size_t len = sizeof(path);
      err = path_join((const char *[]) {prefix, entry->name, NULL}, path, &len, path_behavior_system);
      assert(err == 0);

      memcpy(version, candidate_version, sizeof(dynload_version_t));
    }
  }

  uv_fs_closedir(NULL, &fs, dir, NULL);

  if (!found) return UV_ENOENT;

  if (result) strncpy(result, path, *len);

  *len = strlen(path);

  return 0;

err:
  uv_fs_closedir(NULL, &fs, dir, NULL);

  return err;
}
