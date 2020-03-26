import React from 'react';
import { Link } from 'react-router-dom';
import routes from '../constants/routes.json';
import styles from './Home.css';

import Topbar from './TopBar';
import Games from './Games';
import GameIcon from './GameIcon';

import { topbarIcons } from './Topbar'

var Mousetrap = require('mousetrap');

export default class Home extends React.Component {

  state: {
    topbarSelected: boolean,
    screenIndex: number,
    scr: Array<number>
  }

  constructor(props: any) {
    super(props);
    this.state = {
      topbarSelected: false,
      screenIndex: 0,
      scr: [0, 0]
    }

    // Binding UP and DOWN events
    Mousetrap.bind('w', () => { 
      if(!this.state.topbarSelected)
        this.setState({...this.state, topbarSelected: true});
    });
    Mousetrap.bind('s', () => { 
      if(this.state.topbarSelected)
        this.setState({...this.state, topbarSelected: false});
    });

    // Binding LEFT and RIGHT events
    Mousetrap.bind('a', () => { 
      if(this.state.topbarSelected){
        if(this.state.screenIndex > 0){
          this.setState({...this.state, screenIndex: this.state.screenIndex-1});
        }
      }
      else {
        let temp: Array<number> = this.state.scr.slice();
        temp[this.state.screenIndex] -= 1;
        this.setState({...this.state, scr: temp})
      }
    });
    Mousetrap.bind('d', () => { 
      if(this.state.topbarSelected){
        if(this.state.screenIndex < topbarIcons.length - 1){
          this.setState({...this.state, screenIndex: this.state.screenIndex+1});
        }
      }
      else {
        let temp: Array<number> = this.state.scr;
        temp[this.state.screenIndex] += 1;
        this.setState({...this.state, scr: temp})
      }
    });
  }

  renderBody = () => {
    const bodyScreens = [ <Games selected={this.state.scr[0]}  />, <h2>Lol</h2> ];
    return bodyScreens[this.state.screenIndex];
  }

  render() {
    return (
      <div>
        <Topbar selected={this.state.topbarSelected} screenIndex={this.state.screenIndex} />
        {this.renderBody()}
      </div>
    );
  }
}

/**
 * <div className={styles.container} data-tid="container">
          <h2>Home</h2>
          <Link to={routes.COUNTER}>to Counter</Link>
        </div>
 */