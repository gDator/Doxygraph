#include <string.h>
#include <version.h>

/* - On some systems git is not installed or
 *   installed on a place where FindGit.cmake cannot find it
 * - No git information is present (no .git directory)
 * in those cases clear the gitVersionString (would have string GIT-NOTFOUND).
 */
const char *getGitVersion(void)
{
#define BUF_SIZE 100
  static char gitVersionString[BUF_SIZE];
  static bool init = false;
  if (!init)
  {
    strncpy(gitVersionString,"GIT-NOTFOUND",BUF_SIZE-1);
    strncat(gitVersionString,!strcmp("false","true")?"*":"",BUF_SIZE-1);
    if (!strcmp("GIT-NOTFOUND", "GIT-NOTFOUND")) gitVersionString[0] = '\0';
    gitVersionString[BUF_SIZE-1]='\0';
    init = true;
  }
  return gitVersionString;
}
