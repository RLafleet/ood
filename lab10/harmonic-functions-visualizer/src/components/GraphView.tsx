import { useEffect, useState } from 'react';
import { HarmonicViewModel } from '../viewmodels/HarmonicViewModel';
import * as React from "react";

export const GraphView: React.FC<{ viewModel: HarmonicViewModel }> = ({ viewModel }) => {
    const [data, setData] = useState<{ x: number; y: number }[]>([]);

    useEffect(() => {
        const updateData = () => {
            const graphData = [];
            for (let x = 0; x <= 1; x += 0.01) {
                graphData.push({ x, y: viewModel.evaluate(x) });
            }
            setData(graphData);
        };

        viewModel.subscribe(updateData);
        updateData();
    }, [viewModel]);

    return (
        <svg width="500" height="200" style={{ border: '1px solid black' }}>
            {data.map((point, index) => (
                index > 0 && (
                    <line
                        key={index}
                        x1={data[index - 1].x * 500}
                        y1={100 - data[index - 1].y * 50}
                        x2={point.x * 500}
                        y2={100 - point.y * 50}
                        stroke="blue"
                    />
                )
            ))}
        </svg>
    );
};
