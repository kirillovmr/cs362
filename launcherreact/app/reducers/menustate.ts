import { Action } from 'redux';
import { J_UP, J_DOWN, J_LEFT, J_RIGHT } from '../actions/menustate';

const maxScreens = 2;

var _state = {
    menuSelected: false,
    screenIndex: 0
};

export default function menuState(state = _state, action: Action<string>) {
  switch (action.type) {
    case J_UP:
        if (state.menuSelected)
            return state;
        state.menuSelected = true;
        return state;
        
    case J_DOWN:
        if (!state.menuSelected)
            return state
        state.menuSelected = false;
        return state;

    case J_LEFT:
        if (!state.menuSelected)
            return state;
        if (state.screenIndex <= 0)
            return state;
        state.screenIndex -= 1;
        return state;

    case J_RIGHT:
        if (!state.menuSelected)
            return state;
        if (state.screenIndex >= maxScreens - 1)
            return state;
        state.screenIndex += 1;
        return state;

    default:
        return state;
  }
}
