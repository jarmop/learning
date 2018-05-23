import React, {Component} from 'react';
import './app.css';
import Grid from './grid';
import Code from './code';

const styles = {
  grid: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 40px)',
    gridTemplateColumns: 'repeat(3, 60px)',
    gridGap: '10px',
  },
};

const styles2 = {
  grid: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 40px)',
    gridTemplateColumns: 'repeat(3, 60px)',
    gridGap: '10px',
  },
  b: {
    gridColumn: 2,
    gridRow: 2,
  }
};

class App extends Component {
  render() {
    return (
      <div>
        <div className="reference-row">
          <div className="reference-item">
            <Code cssObject={styles}/>
          </div>
          <div className="reference-item">
            <Grid styles={styles}/>
          </div>
        </div>
        <hr/>
        <div className="reference-row">
          <div className="reference-item">
            <Code cssObject={styles2}/>
          </div>
          <div className="reference-item">
            <div className="grid" style={styles2.grid}>
              <div className="cell">a</div>
              <div className="cell" style={styles2.b}>b</div>
              <div className="cell">c</div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default App;