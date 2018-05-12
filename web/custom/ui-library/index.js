const createElement = (content) => {
  return content
};

const render = (element, container) => {
  container.innerHTML = element;
};

render(
    createElement('dynamic'),
    document.querySelector('#root'),
);