/**
 * Use cases:
 * - DONE add
 * - DONE remove
 * - DONE toggle completion
 * - DONE apply filters
 * - DONE clear completed
 * - show amount of active todos
 * - save to local storage
 */

const KEY_ENTER = 'Enter';

let todoList = document.querySelector('.todo-list');

const clearCompleted = () => {
  document.querySelectorAll('.completed').forEach(listItem => listItem.remove());
};

const applyFilter = (filter) => {
  let handleVisibility = listItem => {listItem.style = 'display: list-item';};
  if (filter === 'completed') {
    handleVisibility = listItem => {
      if (listItem.className === filter) {
        listItem.style = 'display: list-item';
      } else {
        listItem.style = 'display: none';
      }
    };
  } else if (filter === 'active') {
    handleVisibility = listItem => {
      if (!listItem.className) {
        listItem.style = 'display: list-item';
      } else {
        listItem.style = 'display: none';
      }
    };
  }
  document.querySelectorAll('.todo-list li').forEach(handleVisibility);
};

const getUrlFragment = (url) => {
  return url.substring(url.lastIndexOf('/') + 1);
};

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

document.querySelectorAll('.destroy').forEach(destroyButton => {
  bindDestruction(destroyButton);
});

document.querySelectorAll('.toggle').forEach(toggleButton => {
  bindToggle(toggleButton);
});

applyFilter(getUrlFragment(window.location.hash));

document.querySelectorAll('.filters li a').forEach(filterLink => {
  filterLink.onclick = (event) => {
    applyFilter(getUrlFragment(event.target.href));
  };
});

document.querySelector('.clear-completed').onclick = () => {
  clearCompleted();
};