#include "zg_util.h"

#include <iostream>


int usage() {
    std::cerr << "Usage: zapgit init|new|show|ls|fixed\n";
    return 1;
}


int main(int argc, char * argv[]) try {

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
    auto master = wrap(git_reference_dwim, &*repo, "master");
    auto object = wrap(git_reference_peel, &*master, GIT_OBJ_COMMIT);
    auto oid = git_object_id(&*object);
    std::cout << "master = " << oid << "\n";

    auto commit = wrap(git_commit_lookup, &*repo, oid);
    std::cout << "author = " << git_commit_author(&*commit)->name << "\n";
    std::cout << "message = " << git_commit_message(&*commit) << "\n";

} catch (std::exception const & ex) {
    std::cerr << "error: " << ex.what() << "\n";
    return 1;
}
