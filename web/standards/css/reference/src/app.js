import React, {Component} from 'react';
import { BrowserRouter as Router, Route, Link } from "react-router-dom";

import './app.css';
import Grid from './grid';

class App extends Component {
  render() {
    return (
        <Router>
          <div>
            <nav>
              <Link to="/">Home</Link>
              <Link to="/grid">Grid</Link>
            </nav>
            <hr />
            <Route exact path="/" component={Home} />
            <Route path="/grid" component={Grid} />
          </div>
        </Router>
    );
  }
}

const Home = () => <div>Home</div>;

export default App;