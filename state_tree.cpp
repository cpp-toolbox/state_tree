#include "state_tree.hpp"

StateTree::StateTree(std::initializer_list<std::pair<const TreeState, StateTree>> init) {
    for (const auto &pair : init) {
        states_[pair.first] = std::make_shared<StateTree>(pair.second);
    }
}

void StateTree::register_state(TreeState state, std::shared_ptr<StateTree> substate) { states_[state] = substate; }

bool StateTree::is_state_active(std::initializer_list<TreeState> query_states) {
    for (const auto &state : query_states) {
        if (!is_substate_active(state)) {
            return false;
        }
    }
    return true;
}

void StateTree::set_active(const std::vector<TreeState> &path, bool active) {
    if (path.empty()) {
        return;
    }

    set_active_recursive(path, 0, active);
}

void StateTree::print_states(int indent_level, bool is_last) const {
    for (auto it = states_.begin(); it != states_.end(); ++it) {
        bool is_last_in_group = std::next(it) == states_.end();

        std::cout << std::string(indent_level, ' ') << (is_last_in_group ? "└── " : "├── ") << to_string(it->first)
                  << " (" << (it->second->is_active_ ? "Active" : "Inactive") << ")\n";

        it->second->print_states(indent_level + 2, is_last_in_group);
    }
}

StatePath StateTree::get_current_state() const {
    std::vector<TreeState> path;
    build_current_state_path_recursive(path);
    return path;
}

void StateTree::build_current_state_path_recursive(std::vector<TreeState> &path_so_far) const {
    // making a huge assumption that only one path can ever be active at once, probably bad, works for now.
    for (const auto &[tree_state, state_tree] : states_) {
        if (state_tree->is_active_) {
            path_so_far.push_back(tree_state);
            build_current_state_path_recursive(path_so_far);
        }
    }
}

void StateTree::set_active_recursive(const std::vector<TreeState> &path, size_t index, bool active) {
    if (index >= path.size()) {
        return;
    }

    TreeState current_state = path[index];

    if (states_.find(current_state) != states_.end()) {
        states_[current_state]->is_active_ = active;
    }

    if (index + 1 < path.size()) {
        states_[current_state]->set_active_recursive(path, index + 1, active);
    }
}

bool StateTree::is_substate_active(TreeState state) {
    if (states_.find(state) != states_.end() && states_[state]->is_active_) {
        return true;
    }

    for (const auto &substate : states_) {
        if (substate.second->is_substate_active(state)) {
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const StateTree &tree) {
    tree.print_states();
    return os;
}
