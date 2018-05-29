/**
 * Use cases:
 * - DONE add todo
 * - DONE remove todo
 * - DONE toggle todo completion
 * - show active
 * - show completed
 * - clear completed
 */

const KEY_ENTER = 'Enter';

let todoList = document.querySelector('.todo-list');

const bindToggle = (toggleButton) => {
  toggleButton.onclick = (event) => {
    let listItem = event.target.parentElement.parentElement;
    if (listItem.className) {
      listItem.className = '';
    } else {
      listItem.className = 'completed';
    }
  };
};

const bindDestruction = (destroyButton) => {
  destroyButton.onclick = (event) => {
    event.target.parentElement.parentElement.remove();
  };
};

const addTodo = (text) => {
  let toggleButton = document.createElement('input');
  toggleButton.className = 'toggle';
  toggleButton.setAttribute('type', 'checkbox');
  bindToggle(toggleButton);

  let label = document.createElement('label');
  label.innerHTML = text;

  let destroyButton = document.createElement('button');
  destroyButton.className = 'destroy';
  bindDestruction(destroyButton);

  let view = document.createElement('div');
  view.className = 'view';
  view.appendChild(toggleButton);
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

let toggleButtons = document.querySelectorAll('.toggle');
toggleButtons.forEach(toggleButton => {
  bindToggle(toggleButton);
});
