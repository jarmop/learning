import React, {Component} from 'react';
import './app.css';
import Grid from './grid';
import Code from './code';

const styles = {
  grid: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 30px)',
    gridTemplateColumns: 'repeat(3, 60px)',
    gridGap: '10px',
  },
};

const styles2 = {
  grid: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 30px)',
    gridTemplateColumns: 'repeat(3, 60px)',
    gridGap: '10px',
  },
  a: {
    gridColumn: 2,
    gridRow: 3,
  }
};

const styles3 = {
  grid: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 30px)',
    gridTemplateColumns: 'repeat(3, 60px)',
    gridTemplateAreas: `"a a a"
                       "b c c"
                       "b c c"`,
    gridGap: '10px',
  },
  a: {
    gridArea: 'a',
  },
  b: {
    gridArea: 'b',
  },
  c: {
    gridArea: 'c',
  }
};

class App extends Component {
  render() {
    return (
        <div className="reference">
          <div className="reference-item">
            <Code cssObject={styles}/>
          </div>
          <div className="reference-item">
            <Grid styles={styles}/>
          </div>
          <div className="reference-item">
            <Code cssObject={styles2}/>
          </div>
          <div className="reference-item">
            <div className="grid" style={styles2.grid}>
              <div className="cell" style={styles2.a}>a</div>
              <div className="cell"></div>
              <div className="cell"></div>
            </div>
          </div>
          <div className="reference-item">
            <Code cssObject={styles3}/>
          </div>
          <div className="reference-item">
            <div className="grid" style={styles3.grid}>
              <div className="cell" style={styles3.a}>a</div>
              <div className="cell" style={styles3.b}>b</div>
              <div className="cell" style={styles3.c}>c</div>
            </div>
          </div>
        </div>
    );
  }
}

export default App;