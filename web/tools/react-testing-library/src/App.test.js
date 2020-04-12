import React from 'react';
import { render } from '@testing-library/react';
import ReactDOM from 'react-dom';
import App from './App';

// Simple smoke test can be done with just ReactDOM
it('renders without crashing', () => {
  // document is available in jsdom?
  const div = document.createElement('div');
  ReactDOM.render(<App />, div);
});

it('renders learn react link', () => {
  const { getByText } = render(<App />);
  const linkElement = getByText(/testing react testing library/i);
  expect(linkElement).toBeInTheDocument();
});
