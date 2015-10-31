#include "zg_util.h"

#include <iostream>


auto wrap_git_commit_lookup(git_repository *repo, const git_oid *id) {
    return wrap(git_commit_lookup, repo, id);
}


int usage() {
    std::cerr << "Usage: zapgit init|new|show|ls|fixed\n";
    return 1;
}


int main(int argc, char * argv[]) {
    if (argc < 2) {
        return usage();
    }

    git_libgit2_init();
    auto shutdown = onDescoped(git_libgit2_shutdown);

    std::cerr << "zapgit 0.1";

    {
        int major, minor, rev;
        git_libgit2_version(&major, &minor, &rev);
        std::cerr << " (libgit2 " << major << "." << minor << "." << rev << ")\n";
    }

    auto repo = wrap(git_repository_open, ".");
    auto master = wrap(git_reference_lookup, &*repo, "refs/heads/master");
}
