import React, {Component} from 'react';
import './app.css';
import Grid from './grid';
import Code from './code';

const styles = {
  gridStyle: {
    display: 'grid',
    gridTemplateRows: 'repeat(3, 100px)',
    gridTemplateColumns: 'repeat(3, 100px)',
    gridGap: '10px',
  },
  cellStyle: {
    backgroundColor: 'grey',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    fontSize: '30px',
    cursor: 'pointer',
    color: 'white',
  },
};

class App extends Component {
  render() {
    return (
        <div className="reference-row">
          <div className="reference-item">
            <Code/>
          </div>
          <div className="reference-item">
            <Grid styles={styles}/>
          </div>
        </div>
    );
  }
}

export default App;