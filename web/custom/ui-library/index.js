const createElement = (type, props, children) => {
  if (typeof type === 'function') {
    return type(props);
  }

  if (Array.isArray(children)) {
    children = children.join('');
  }

  return '<' + type + '>' + children + '</' + type + '>';
};

const DateTime = (props) => {
  return createElement('time', null, (new Date).toLocaleString());
};

const Header = (props) => {
  return createElement('header', null, [
      createElement('h1', null, 'Title'),
      createElement(DateTime),
  ]);
};

const App = (props) => {
  return createElement('article', null, [
        createElement(Header),
        createElement('p', null, 'Paragraph 1'),
        createElement('p', null, 'Paragraph 2'),
      ]);
};

const render = (content, container) => {
  container.innerHTML = content;
};

render(
    createElement(App),
    document.querySelector('#root'),
);