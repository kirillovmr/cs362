import React from 'react';
import styles from './InspiredBy.css';

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faHandPointRight, faHandPointLeft } from '@fortawesome/free-solid-svg-icons'

import ParticleImage, { ParticleOptions, Vector } from "react-particle-image";
import theysImg from '../../resources/theys.jpg';

function componentToHex(c: number) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

const particleOptions: ParticleOptions = {
    filter: ({ x, y, image }) => {
      // Get pixel
      const pixel = image.get(x, y);
      // Make a particle for this pixel if blue > 50 (range 0-255)
      return pixel.b < 140;
      return true;
    },
    color: ({ x, y, image }) => {
        const pixel = image.get(x, y);
        return "#" + componentToHex(pixel.r) + componentToHex(pixel.g) + componentToHex(pixel.b);
    },
    mass: () => 30,
    radius: ({ x, y, image }) => {
        const pixel = image.get(x, y);
        const magnitude = (pixel.r + pixel.g + pixel.b) / 3;
        // Lighter colors will have smaller radius
        return 2 - (magnitude / 255) * 1.5;
    },
    friction: () => 0.15,
    // initialPosition: ({ canvasDimensions }) => {
    //     return new Vector(canvasDimensions.width / 2, canvasDimensions.height / 2);
    // }
};

export default function InspiredBy(props: any) {
    

    return (
        <div className={styles.container}>
            <ParticleImage
                src={theysImg}
                width={300}
                height={300}
                scale={1.5}
                entropy={props.shiftValue}
                maxParticles={5000}
                particleOptions={particleOptions}
                backgroundColor={"transparent"}
            />

            <h5>inspired by</h5>
            <h5>
                <FontAwesomeIcon icon={faHandPointRight} />
                Mitchell Theys
                <FontAwesomeIcon icon={faHandPointLeft} />
            </h5>
        </div>
        
        // <div className={styles.theys}></div>
    );
}