# state_tree
SUBPROJECT

# operation
In order to use this subproject, you must make an additional subproject that defines the enum `TreeState` in here you will create your own custom states for your state tree, it's fine if this enum contains all your states in a disorganized manner, the `StateTree` will define the states themselves, this enum just allows you to operate on the tree in a safer manner so that you don't have to use strings or something like that.

Create a new directory somewhere called `tree_states` at a minimum create the following files:

`tree_states/tree_states.hpp`
```cpp
#ifndef TREE_STATES_HPP
#define TREE_STATES_HPP

enum class TreeState {
    // TODO remove the dummy state and fill in with all the states you need
    DUMMY_STATE
};

#endif // TREE_STATES_HPP
```

`tree_states/tree_states.hpp`
```cpp
#include "tree_states.hpp"

const char *to_string(TreeState state) {
    switch (state) {
    case TreeState::DUMMY_STATE:
        return "DUMMY_STATE";
    default:
        return "UNKNOWN";
    }
}
```

`tree_states/sbpt.ini`
```ini
[subproject]
export = tree_states.hpp
tags = utility
```

**Note**: We integrate with `spbt` so that we don't have to include hardcoded includes into `state_tree.hpp` so that it remains portable

# usage
```cpp
int main() {
    // Define states recursively using initializer lists
    StateTree state({
        {State::MAIN_MENU, StateTree({{State::SETTINGS_MENU, StateTree({{State::GRAPHICS, StateTree({})}, {State::INPUT, StateTree({})}})}})}
    });

    std::cout << "Initial StateTree structure:\n";
    std::cout << state << std::endl;

    // Setting states as active using the recursive path approach
    std::vector<State> path = {State::MAIN_MENU, State::SETTINGS_MENU, State::GRAPHICS};
    state.set_active(path, true);

    // Check if a specific state is active (checking parent-child relationship) inline
    bool active = state.is_state_active({State::MAIN_MENU, State::SETTINGS_MENU, State::GRAPHICS});
    std::cout << "Is the state active? " << (active ? "Yes" : "No") << std::endl;

    // Check with another query, where we check for "MAIN_MENU" even when in "INPUT"
    active = state.is_state_active({State::MAIN_MENU, State::SETTINGS_MENU, State::INPUT});
    std::cout << "Is the state active? " << (active ? "Yes" : "No") << std::endl;

    return 0;
}
```

