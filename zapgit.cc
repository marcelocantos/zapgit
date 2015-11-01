#include "libgit2pp/git2pp.h"

#include <iostream>


void prologue() {
    std::cerr << "zapgit 0.1";

    int major, minor, rev;
    git_libgit2_version(&major, &minor, &rev);
    std::cerr << " (libgit2 " << major << "." << minor << "." << rev << ")\n";
}


int usage() {
    std::cerr << "Usage: zapgit init|new|show|ls|fixed\n";
    return 1;
}


bool git2ok(int rc) {
    if (rc >= 0) {
        return true;
    }
    const git_error *e = giterr_last();
    fprintf(stderr, "Error %d/%d: %s\n", rc, e->klass, e->message);
    return false;
}


int main(int argc, char * argv[]) try {
    prologue();

    if (argc < 2) {
        return usage();
    }

    git2pp::Session git2;

    auto repo = git2[git_repository_open](".");
    auto master = repo[git_reference_dwim]("master");
    auto commit = master[git_reference_peel](GIT_OBJ_COMMIT).as<git_commit>();
    auto dup = commit;
    dup = commit;
    std::cout << "master = " << commit[git_commit_id]() << "\n";
    std::cout << "author = " << commit[git_commit_author]()->name << "\n";
    std::cout << "message = " << commit[git_commit_message]() << "\n";
} catch (std::exception const & ex) {
    std::cerr << ex.what() << "\n";
    return 1;
} catch (...) {
    std::cerr << "unknown exception\n";
    return 1;
}
