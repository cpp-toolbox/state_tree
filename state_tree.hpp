#ifndef STATETREE_HPP
#define STATETREE_HPP

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <initializer_list>

#include "sbpt_generated_includes.hpp"

using StatePath = const std::vector<TreeState>;

const char *to_string(TreeState state);

class StateTree {
  public:
    using StateMap = std::unordered_map<TreeState, std::shared_ptr<StateTree>>;

    StateTree(std::initializer_list<std::pair<const TreeState, StateTree>> init);
    void register_state(TreeState state, std::shared_ptr<StateTree> substate = nullptr);
    bool is_state_active(std::initializer_list<TreeState> query_states);
    void set_active(const std::vector<TreeState> &path, bool active);
    StatePath get_current_state() const;
    void build_current_state_path_recursive(std::vector<TreeState> &path_so_far) const;
    void print_states(int indent_level = 0, bool is_last = false) const;

  private:
    void set_active_recursive(const std::vector<TreeState> &path, size_t index, bool active);
    bool is_substate_active(TreeState state);
    StateMap states_;
    bool is_active_ = false; // Default state is inactive
};

std::ostream &operator<<(std::ostream &os, const StateTree &tree);

#endif // STATETREE_HPP
