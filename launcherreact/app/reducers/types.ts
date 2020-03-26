import { Dispatch as ReduxDispatch, Store as ReduxStore, Action } from 'redux';

export type counterStateType = {
  counter: number;

  /** Joystick */
  topBarSelected: boolean;
};

export type menuStateType = {
  menuSelected: boolean,
  screenIndex: number
};

export type GetState = () => counterStateType;

export type Dispatch = ReduxDispatch<Action<string>>;

export type Store = ReduxStore<counterStateType, Action<string>>;
