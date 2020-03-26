import React from 'react';
import styles from './Settings.css';
  

export default function Settings(props: any) {

    const renderConnectionStatus = () => {
        if (props.wifiState.connected) {
            return (
                <span>
                    {`connected to network "${props.wifiState.ssid}" with strength ${props.wifiState.quality}%.`}
                </span>
            );
        }
        return <span>disconnected.</span>;
    }

    return (
        <div className={styles.container} data-tid="container">
            <h3 className={styles.shadow}>Settings</h3>
            <div className={styles.settingsList}>
                <h4>- Network status: {renderConnectionStatus()}</h4>
                <h4>- Server status: disconnected.</h4>
            </div>
        </div>
      );
}