#include "state_tree.hpp"

const char* to_string(State state) {
    switch (state) {
        case State::MAIN_MENU: return "MAIN_MENU";
        case State::SETTINGS_MENU: return "SETTINGS_MENU";
        case State::GRAPHICS: return "GRAPHICS";
        case State::INPUT: return "INPUT";
        default: return "UNKNOWN";
    }
}

StateTree::StateTree(std::initializer_list<std::pair<const State, StateTree>> init) {
    for (const auto& pair : init) {
        states_[pair.first] = std::make_shared<StateTree>(pair.second);
    }
}

void StateTree::register_state(State state, std::shared_ptr<StateTree> substate) {
    states_[state] = substate;
}

bool StateTree::is_state_active(std::initializer_list<State> query_states) {
    for (const auto& state : query_states) {
        if (!is_substate_active(state)) {
            return false;
        }
    }
    return true;
}

void StateTree::set_active(const std::vector<State>& path, bool active) {
    if (path.empty()) {
        return;
    }

    set_active_recursive(path, 0, active);
}

void StateTree::print_states(int indent_level, bool is_last) const {
    for (auto it = states_.begin(); it != states_.end(); ++it) {
        bool is_last_in_group = std::next(it) == states_.end();
        
        std::cout << std::string(indent_level, ' ') 
                  << (is_last_in_group ? "└── " : "├── ")
                  << to_string(it->first) << " ("
                  << (it->second->is_active_ ? "Active" : "Inactive") << ")\n";

        it->second->print_states(indent_level + 2, is_last_in_group);
    }
}

void StateTree::set_active_recursive(const std::vector<State>& path, size_t index, bool active) {
    if (index >= path.size()) {
        return;
    }

    State current_state = path[index];

    if (states_.find(current_state) != states_.end()) {
        states_[current_state]->is_active_ = active;
    }

    if (index + 1 < path.size()) {
        states_[current_state]->set_active_recursive(path, index + 1, active);
    }
}

bool StateTree::is_substate_active(State state) {
    if (states_.find(state) != states_.end() && states_[state]->is_active_) {
        return true;
    }

    for (const auto& substate : states_) {
        if (substate.second->is_substate_active(state)) {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const StateTree& tree) {
    tree.print_states();
    return os;
}
