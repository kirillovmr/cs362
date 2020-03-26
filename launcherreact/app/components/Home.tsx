import React from 'react';
import Topbar, { topbarIcons } from './TopBar';
import InspiredBy from './InspiredBy';
import Settings from './Settings';
import Games from './Games';

var Mousetrap = require('mousetrap');
var wifi = require("node-wifi");

export default class Home extends React.Component {
  state: {
    wifi: {
      connected: boolean,
      ssid: string,
      quality: number
    },

    topbarSelected: boolean,
    screenIndex: number,
    shiftDx: Array<number>
    shiftValue: Array<number>,
    pressValue: Array<number>
  }
  checkWifiInterval: NodeJS.Timeout;

  constructor(props: any) {
    super(props);
    this.state = {
      wifi: {
        connected: false,
        ssid: "",
        quality: 0
      },
      topbarSelected: false,
      screenIndex: 1,
      shiftDx: [1, 1, 5],
      shiftValue: [0, 0, 10],
      pressValue: [0, 0, 0]
    }

    wifi.init({
      iface: null // network interface, choose a random wifi interface if set to null
    });

    // Check for wifi
    this.checkWifiInterval = setInterval(this.getCurrentConnection, 5000);

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
        let temp: Array<number> = this.state.shiftValue.slice();
        temp[this.state.screenIndex] -= this.state.shiftDx[this.state.screenIndex];
        this.setState({...this.state, shiftValue: temp})
      }
    });
    Mousetrap.bind('d', () => { 
      if(this.state.topbarSelected){
        if(this.state.screenIndex < topbarIcons.length - 1){
          this.setState({...this.state, screenIndex: this.state.screenIndex+1});
        }
      }
      else {
        let temp: Array<number> = this.state.shiftValue;
        temp[this.state.screenIndex] += this.state.shiftDx[this.state.screenIndex];;
        this.setState({...this.state, shiftValue: temp})
      }
    });

    // Binding enter
    Mousetrap.bind('e', () => { 
      if(!this.state.topbarSelected) {
        let temp: Array<number> = this.state.pressValue.slice();
        temp[this.state.screenIndex] += 1;
        this.setState({...this.state, pressValue: temp});
      }
    });
  }

  getCurrentConnection = () => {
    wifi.getCurrentConnections((err: any, currentConnections: any) => {
      if (err) {
        console.log("Error getting wifi connection", err);
      }
      console.log("Wifi connections", currentConnections);

      if (currentConnections.length > 0) {
        this.setState({
          ...this.state, 
          wifi: {
            ...this.state.wifi, 
            connected: true,
            ssid: currentConnections[0].ssid,
            quality: currentConnections[0].quality
          }
        });
        return currentConnections;
      }
      return null;
    });
  }

  renderTopbar = () => {
    return (<Topbar 
      selected={this.state.topbarSelected} 
      screenIndex={this.state.screenIndex} 
      wifiConnected={this.state.wifi.connected}
    />);
  }

  renderBody = () => {
    return [ 
      <Settings 
        wifiState={this.state.wifi}
      />,
      <Games 
        shiftValue={this.state.shiftValue[1]} 
        pressValue={this.state.pressValue[1]}  
        topbarSelected={this.state.topbarSelected}
      />, 
      <InspiredBy
        shiftValue={this.state.shiftValue[2]}
      />
    ][this.state.screenIndex];
  }

  render() {
    return (
      <div>
        {this.renderTopbar()}
        {this.renderBody()}
      </div>
    );
  }
}