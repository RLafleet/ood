export class ITabSwitcher {
    private activeView: 'graph' | 'table' = 'graph';
    private listeners: (() => void)[] = [];

    getActiveView(): 'graph' | 'table' {
        return this.activeView;
    }

    switchToGraphView(): void {
        this.activeView = 'graph';
        this.notifyListeners();
    }

    switchToTableView(): void {
        this.activeView = 'table';
        this.notifyListeners();
    }

    subscribe(listener: () => void): void {
        this.listeners.push(listener);
    }

    private notifyListeners(): void {
        this.listeners.forEach((listener) => listener());
    }
}
