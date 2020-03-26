export const J_UP = 'J_UP';
export const J_DOWN = 'J_DOWN';
export const J_LEFT = 'J_LEFT';
export const J_RIGHT = 'J_RIGHT';

export function jup() {
  return {
    type: J_UP
  }
}
export function jdown() {
  return {
    type: J_DOWN
  }
}
export function jleft() {
  return {
    type: J_LEFT
  }
}
export function jright() {
  return {
    type: J_RIGHT
  }
}