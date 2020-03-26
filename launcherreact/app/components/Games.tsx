import React from 'react';
import styles from './Games.css';

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faWifi } from '@fortawesome/free-solid-svg-icons'

import gameList from '../../resources/games/games.json'

import GameIcon from './GameIcon';
  

// Keep track of the direction we moved to
let oldShiftValue = 0;
let oldPressValue = 0;

export default function Games(props: any) {

    let games = gameList;

    const renderGameIcons = () => {
        // Moving left
        if (props.shiftValue < oldShiftValue) {
            games.unshift(games.pop()!);
        }
        // Moving right
        if (props.shiftValue > oldShiftValue) {
            games.push(games.shift()!);
        }
        // Press ok
        if (props.pressValue > oldPressValue) {
            console.log(`Starting ${games[0].name}`);
        }

        // Remembering last value
        oldShiftValue = props.shiftValue;
        oldPressValue = props.pressValue;

        return games.map((object, i) => {
            if (i < 3) {
                return <GameIcon 
                    object={object} index={i} key={i}
                    topbarSelected={props.topbarSelected} 
                />
            }
            return;
        })
    };

    return (
        <div className={styles.container} data-tid="container">
            <h3 className={styles.shadow}>Games</h3>
            <div>
                {renderGameIcons()}
                <div className={styles.clear}></div>
            </div>
            
            <h5 className={styles.dummyText}>lol</h5>
            <h5 className={`${styles.onlineText} ${styles.inline}`}>{gameList[0].name}
                {games[0].isWifiRequired ? <FontAwesomeIcon icon={faWifi} className={`${styles.inline} ${styles.shadow}`} /> : ""}
            </h5>
            <h6 className={styles.inline}>{games[0].isWifiRequired ? "( network connection required )" : ""}</h6>
            <h5 className={styles.descrText}>{gameList[0].description}</h5>
        </div>
      );
}