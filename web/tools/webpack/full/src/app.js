import React from 'react';
import { BrowserRouter as Router, Route, Link } from "react-router-dom";

import Home from './home';
import Section from './section';

const ROUTE = {
  HOME: '/',
  SECTION: '/section',
};

class App extends React.Component {
  render() {
    return (
        <Router>
          <div>
            <ul>
              <li>
                <Link to={ROUTE.HOME}>Home</Link>
              </li>
              <li>
                <Link to={ROUTE.SECTION}>A large section</Link>
              </li>
            </ul>
            <Route exact path="/" component={Home} />
            <Route exact path="/section" component={Section} />
          </div>
        </Router>
    );
  }
}

export default App;