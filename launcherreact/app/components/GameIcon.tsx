import React from 'react';
import styles from './GameIcon.css';

import pacmanImg from '../../resources/games/icons/pacman.png';
import cocImg from '../../resources/games/icons/clashofclans.png';
import flappyBirdImg from '../../resources/games/icons/flappybird.png';

// let a = require('./../resources/games/icons/flappybird.png')

const getImage = (name: String) => {
    switch(name) {
        case "Clash of Clans":
            return cocImg;
        case "PAC-MAN 256":
            return pacmanImg;
        case "Flappy Bird":
            return flappyBirdImg;
    }
}

const getSelectedClass = (name: String) => {
    switch (name) {
        case "Clash of Clans":
            return styles.selectedRed;
        case "PAC-MAN 256":
            return styles.selectedYellow;
        case "Flappy Bird":
            return styles.selectedGreen;
        default:
            return styles.selectedRed;
    }
}

export default function GameIcon(props: any) {

    const borderClass = `${props.index == 0 ? `${getSelectedClass(props.object.name)}` : ``}`;

    return (
        <div className={`${styles.container} `} data-tid="container">
            <img src={getImage(props.object.name)} className={`${borderClass}`} />
            {/* <img src="./games/icons/flappybird.png" className={`${borderClass}`} /> */}
        </div>
    );
}