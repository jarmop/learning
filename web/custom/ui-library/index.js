const createElement = (type, props, children) => {
  if (typeof type === 'function') {
    return type(props);
  }

  return '<' + type + '>' + children + '</' + type + '>';
};

const App = (props) => {
  return createElement('p', props, 'dynamic');
};

const render = (content, container) => {
  container.innerHTML = content;
};

render(
    createElement(App),
    document.querySelector('#root'),
);