import React, {Component} from 'react';
import './grid.css';
import Code from './code';

const styles = {
  grid: {
    display: 'grid',
    gridTemplateColumns: 'repeat(3, auto)',
    // gap: '10px',
  },
};

const styles2 = {
  grid: {
    display: 'grid',
    gridTemplateColumns: 'repeat(3, auto)',
    // gap: '10px',
  },
  a: {
    gridColumn: 3,
    gridRow: 3,
  },
  b: {
    gridColumn: 2,
    gridRow: 2,
  },
};

const styles5 = {
  grid: {
    display: 'grid',
    gridTemplateColumns: 'minmax(80px, auto) minmax(30px, 100px)',
    // gap: '10px 0',
  },
  c: {
    gridColumn: '1 / 3',
  },
};

const styles3 = {
  grid: {
    display: 'grid',
    gridTemplateColumns: 'repeat(3, auto)',
    gridTemplateRows: 'repeat(3, 30px)',
    gridTemplateAreas: `"a a a"
                       "b c c"
                       "b c c"`,
    // gap: '10px',
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

const styles4 = {
  grid: {
    display: 'grid',
    gridTemplateColumns: 'repeat(3, auto)',
  },
};

class Grid extends Component {
  render() {
    return (
        <div className="reference">
          <h2 className="reference-header">You can do this</h2>
          <div className="reference-item">
            <Code cssObject={styles}/>
          </div>
          <div className="result">
            <div className="grid" style={styles.grid}>
              <div className="cell">a</div>
              <div className="cell">b</div>
              <div className="cell">c</div>
              <div className="cell">d</div>
            </div>
          </div>
          <div className="reference-item">
            <Code cssObject={styles4}/>
          </div>
          <div className="result">
            <div className="grid" style={styles4.grid}>
              <div className="cell">aaa<br/>aaa</div>
              <div className="cell">a</div>
              <div className="cell">aa</div>
              <div className="cell">a</div>
              <div className="cell">aaaa</div>
              <div className="cell">a</div>
              <div className="cell">a</div>
              <div className="cell">a</div>
              <div className="cell">a<br/>a<br/>a</div>
            </div>
          </div>
          <div className="reference-item">
            <Code cssObject={styles5}/>
          </div>
          <div className="result">
            <div className="grid" style={styles5.grid}>
              <div className="cell">a</div>
              <div className="cell">b</div>
              <div className="cell" style={styles5.c}>c</div>
              <div className="cell">d</div>
              <div className="cell">e</div>
            </div>
          </div>
          <h2 className="reference-header">You can also do this (but why would you)</h2>
          <div className="reference-item">
            <Code cssObject={styles2}/>
          </div>
          <div className="result">
            <div className="grid" style={styles2.grid}>
              <div className="cell" style={styles2.a}>a</div>
              <div className="cell" style={styles2.b}>b</div>
              <div className="cell">c</div>
            </div>
          </div>
          <div className="reference-item">
            <Code cssObject={styles3}/>
          </div>
          <div className="result">
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

export default Grid;