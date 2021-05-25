import UIKit

protocol Observer {
    func update()
}

protocol Observable {
    func addObserver(observer: Observer)
    func notifyObservers()
}

enum Command {
    case MoveUp
    case MoveDown
    case Stop
}

protocol CommandProvider: Observable {
    var command: Command { get }
}

protocol Controller {
    func moveUp()
    func moveDown()
    func stop()
}

class Handset: Controller {
    
    private var moveUpAction: ControllerAction
    private var moveDownAction: ControllerAction
    private var stopAction: ControllerAction
    
    init(moveUpAction: ControllerAction, moveDownAction: ControllerAction, stopAction: ControllerAction) {
        self.moveUpAction = moveUpAction
        self.moveDownAction = moveDownAction
        self.stopAction = stopAction
    }
    
    func moveUp() {
        moveUpAction.perform()
    }
    
    func moveDown() {
        moveDownAction.perform()
    }
    
    func stop() {
        stopAction.perform()
    }
}

class Webserver: CommandProvider {
    
    var command: Command = .Stop
    private var observers = [Observer]()
    
    func addObserver(observer: Observer) {
        observers.append(observer)
    }
    
    func notifyObservers() {
        for observer in observers {
            observer.update()
        }
    }
    
    func moveDownPage() {
        command = .Stop
        notifyObservers()
    }
    
    func stopPage() {
        command = .MoveDown
        notifyObservers()
    }
    
    func moveUpPage() {
        command = .MoveUp
        notifyObservers()
    }
}

protocol Action {
    var pin: Int { get }
    var perform: () -> () { get }
}

struct ControllerAction: Action {
    var pin: Int
    var perform: () -> ()
}


struct System: Observer {
    var commandProvider: CommandProvider
    var controller: Controller
    
    init(commandProvider: CommandProvider, controller: Controller) {
        self.controller = controller
        self.commandProvider = commandProvider
        self.commandProvider.addObserver(observer: self)
    }
    
    func update() {
        switch commandProvider.command {
        case .MoveDown:
            controller.moveDown()
        case .MoveUp:
            controller.moveUp()
        case .Stop:
            controller.stop()
        }
    }
}

let moveUpAction = ControllerAction(pin: 14) {
    print("14")
}
let moveDownAction = ControllerAction(pin: 15) {
    print("15")
}
let stopAction = ControllerAction(pin: 16) {
    print("16")
}

var webserver = Webserver()
var system = System(commandProvider: webserver, controller: Handset(moveUpAction: moveUpAction, moveDownAction: moveDownAction, stopAction: stopAction))

webserver.moveDownPage()
webserver.moveUpPage()
webserver.stopPage()





