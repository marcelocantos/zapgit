#include <git2.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>


#define CHECK_GIT2(code) do { \
        auto rc = (code); \
        if (rc < 0) { \
            std::string msg = #code; \
            auto err = giterr_last(); \
            if (err) { \
                msg += ": "; \
                msg += err->message; \
            } \
            throw std::runtime_error(msg); \
        } \
    } while (false)

template <typename F>
class OnDescoped {
public:
    OnDescoped(F descoped)
    : descoped_{std::make_unique<F>(std::move(descoped))}
    { }

    OnDescoped(OnDescoped &&) = default;

    ~OnDescoped() {
        if (descoped_) {
            try {
                (*descoped_)();
            } catch (...) {
                // What can you do?
            }
        }
    }

private:
    std::unique_ptr<F> descoped_;
};


template <typename F>
auto onDescoped(F descoped) {
    return OnDescoped<F>{descoped};
}


template <typename F>
struct function_arg0;


template <typename R, typename... Args>
struct function_arg0<R (*)(Args...)> {
    using type = typename std::tuple_element<0, std::tuple<Args...>>::type;
};


template <typename T> struct git_free_func {};

#define GIT_FREE_FUNC(type) \
    template <> struct git_free_func<type> { \
        using func_t = void (*)(type *); \
        static func_t func() { return type##_free; } \
    };

GIT_FREE_FUNC(git_annotated_commit);
GIT_FREE_FUNC(git_blame);
GIT_FREE_FUNC(git_blob);
GIT_FREE_FUNC(git_branch_iterator);
GIT_FREE_FUNC(git_buf);
GIT_FREE_FUNC(git_commit);
GIT_FREE_FUNC(git_config);
GIT_FREE_FUNC(git_config_entry);
GIT_FREE_FUNC(git_config_iterator);
//GIT_FREE_FUNC(git_cred);
GIT_FREE_FUNC(git_describe_result);
GIT_FREE_FUNC(git_diff);
GIT_FREE_FUNC(git_diff_stats);
GIT_FREE_FUNC(git_filter_list);
//GIT_FREE_FUNC(git_hashsig);
GIT_FREE_FUNC(git_index);
GIT_FREE_FUNC(git_index_conflict_iterator);
GIT_FREE_FUNC(git_indexer);
GIT_FREE_FUNC(git_merge_file_result);
GIT_FREE_FUNC(git_note);
GIT_FREE_FUNC(git_note_iterator);
GIT_FREE_FUNC(git_object);
GIT_FREE_FUNC(git_odb);
GIT_FREE_FUNC(git_odb_object);
GIT_FREE_FUNC(git_odb_stream);
GIT_FREE_FUNC(git_oid_shorten);
GIT_FREE_FUNC(git_oidarray);
GIT_FREE_FUNC(git_packbuilder);
GIT_FREE_FUNC(git_patch);
GIT_FREE_FUNC(git_pathspec);
GIT_FREE_FUNC(git_pathspec_match_list);
GIT_FREE_FUNC(git_rebase);
GIT_FREE_FUNC(git_refdb);
GIT_FREE_FUNC(git_reference);
GIT_FREE_FUNC(git_reference_iterator);
GIT_FREE_FUNC(git_reflog);
GIT_FREE_FUNC(git_remote);
GIT_FREE_FUNC(git_repository);
GIT_FREE_FUNC(git_revwalk);
GIT_FREE_FUNC(git_signature);
GIT_FREE_FUNC(git_status_list);
GIT_FREE_FUNC(git_strarray);
GIT_FREE_FUNC(git_submodule);
GIT_FREE_FUNC(git_tag);
GIT_FREE_FUNC(git_tree);
GIT_FREE_FUNC(git_tree_entry);
GIT_FREE_FUNC(git_treebuilder);


template <typename F, typename... Args>
auto wrap(F f, Args &&... args) {
    using OutPP = typename function_arg0<F>::type;
    using OutP = std::remove_pointer_t<OutPP>;
    using Out = std::remove_pointer_t<OutP>;
    OutP outp;
    CHECK_GIT2(f(&outp, std::forward<Args>(args)...));
    return std::unique_ptr<Out, void (*)(OutP)>{
        outp, git_free_func<Out>::func()};
}
