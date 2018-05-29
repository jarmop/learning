/**
 * Use cases:
 * - DONE add todo
 * - remove todo
 * - set todo completed
 * - unset todo completed
 * - show active
 * - show completed
 * - clear completed
 */

const KEY_ENTER = 'Enter';

let todoList = document.querySelector('.todo-list');

const addTodo = (text) => {
  let listItem = document.createElement('li');
  listItem.innerHTML = `
<div class="view">
  <input class="toggle" type="checkbox">
  <label>${text}</label>
  <button class="destroy"></button>
</div>
`;
  todoList.appendChild(listItem);
};

window.onkeypress = (event) => {
  if (event.key === KEY_ENTER) {
    addTodo(event.target.value);
    event.target.value = '';
  }
};
