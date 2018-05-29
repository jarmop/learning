/**
 * Use cases:
 * - DONE add todo
 * - DONE remove todo
 * - set todo completed
 * - unset todo completed
 * - show active
 * - show completed
 * - clear completed
 */

const KEY_ENTER = 'Enter';

let todoList = document.querySelector('.todo-list');

const bindDestruction = (destroyButton) => {
  destroyButton.onclick = (event) => {
    event.target.parentElement.parentElement.remove();
  };
};

const addTodo = (text) => {
  let input = document.createElement('input');
  input.className = 'toggle';
  input.setAttribute('type', 'checkbox');

  let label = document.createElement('label');
  label.innerHTML = text;

  let destroyButton = document.createElement('button');
  destroyButton.className = 'destroy';
  bindDestruction(destroyButton);

  let view = document.createElement('div');
  view.className = 'view';
  view.appendChild(input);
  view.appendChild(label);
  view.appendChild(destroyButton);

  let listItem = document.createElement('li');
  listItem.appendChild(view);

  todoList.appendChild(listItem);
};

window.onkeypress = (event) => {
  if (event.key === KEY_ENTER) {
    addTodo(event.target.value);
    event.target.value = '';
  }
};

let destroyButtons = document.querySelectorAll('.destroy');
destroyButtons.forEach(destroyButton => {
  bindDestruction(destroyButton);
});

