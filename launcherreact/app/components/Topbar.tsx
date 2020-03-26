import React from 'react';
import styles from './Topbar.css';

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faWifi, faDiceD6, faHeart, faServer } from '@fortawesome/free-solid-svg-icons'


import icon from '../../resources/players/ana.png';

//https://overwatch.gamepedia.com/Player_Icons

// Icons to be used in top menu
export const topbarIcons = [ faDiceD6, faHeart ];

export default function Topbar(props: any) {

    const wifiClass = `${styles.inline} ${styles.shadow} ${styles.iconDisconnected}`;
    const serverClass = `${styles.inline} ${styles.shadow} ${styles.iconDisconnected}`;

    const renderCenter = () => {
        return topbarIcons.map((icon, i) => {
            return <FontAwesomeIcon icon={icon} className={`${styles.psIcon} ${styles.shadow} ${props.screenIndex == i ? styles.active : ""} ${props.screenIndex == i && props.selected ? styles.selected : ""}`} />
        });
    };

    return (
        <div className={styles.container} data-tid="container">
            <div className={styles.oneThreeLeftContainer}>
                <img className={`${styles.playerIcon} ${styles.shadow}`} src={icon}></img>
                <span>
                    <h5 className={`${styles.inline} ${styles.shadow}`}>Player</h5>
                </span>
            </div>

            <div className={styles.oneThreeRightContainer}>
                <FontAwesomeIcon icon={faServer} className={serverClass} />
                <FontAwesomeIcon icon={faWifi} className={wifiClass} />
                <h5 className={`${styles.inline} ${styles.shadow}`}>13:56</h5>
            </div>

            <div className={styles.oneThreeCenterContainer}>
                {renderCenter()}
            </div>
        </div>
    );
}