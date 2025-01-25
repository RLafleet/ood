import { useState, useEffect } from 'react';
import { GraphView } from './GraphView';
import { TableView } from './TableView';
import { HarmonicViewModel } from '../viewmodels/HarmonicViewModel';
import { ITabSwitcher } from './ITabSwitcher.ts';
import * as React from "react";

export const TabSwitcher: React.FC<{ viewModel: HarmonicViewModel }> = ({ viewModel }) => {
    const viewManager = new ITabSwitcher();
    const [activeView, setActiveView] = useState<'graph' | 'table'>(viewManager.getActiveView());

    useEffect(() => {
        const updateView = () => setActiveView(viewManager.getActiveView());
        viewManager.subscribe(updateView);
    }, [viewManager]);

    return (
        <div>
            <div style={{ marginBottom: '1rem' }}>
                <button onClick={() => viewManager.switchToGraphView()} style={{ marginRight: '1rem' }}>
                    Graph View
                </button>
                <button onClick={() => viewManager.switchToTableView()}>Table View</button>
            </div>
            {activeView === 'graph' ? (
                <GraphView viewModel={viewModel} />
            ) : (
                <TableView viewModel={viewModel} />
            )}
        </div>
    );
};
