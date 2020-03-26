import React from 'react';
import styles from './Games.css';

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faWifi } from '@fortawesome/free-solid-svg-icons'

import gameList from '../../resources/games/games.json'

import GameIcon from './GameIcon';
  

// Keep track of the direction we moved to
let oldValue = 0;

export default function Games(props: any) {

    let games = gameList;

    const renderGameIcons = () => {
        // Moving left
        if (props.selected < oldValue) {
            games.unshift(games.pop()!);
        }
        // Moving right
        if (props.selected > oldValue) {
            games.push(games.shift()!);
        }
        // Remembering last value
        oldValue = props.selected;

        return games.map((object, i) => {
            return <GameIcon object={object} index={i} key={i}/>
        })
    };

    return (
        <div className={styles.container} data-tid="container">
            <h3 className={styles.shadow}>Games</h3>
            <div>
                {renderGameIcons()}
            </div>
            
            <h5 className={styles.dummyText}>lol</h5>
            <h5 className={`${styles.onlineText} ${styles.inline}`}>{gameList[0].name}</h5>
                {games[0].isWifiRequired ? <FontAwesomeIcon icon={faWifi} className={`${styles.inline} ${styles.shadow}`} /> : ""}
            <h5 className={styles.descrText}>{gameList[0].description}</h5>
        </div>
      );
}