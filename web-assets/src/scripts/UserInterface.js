export class UserInterface {
  /**
   * @param {Object} options
   */
  constructor(options) {
    this.defaultLocalIp = options.defaultLocalIp;
    this.defaultHttpPort = options.defaultHttpPort;
    this.defaultMakeCoffeeSeconds = options.defaultMakeCoffeeSeconds;

    this.localIpStorageKey = options.localIpStorageKey;
    this.httpPortStorageKey = options.httpPortStorageKey;
    this.makeCoffeeSecondsStorageKey = options.makeCoffeeSecondsStorageKey;

    this.activeStateClassName = options.activeStateClassName;
  }

  /**
   * @public
   * @param {(localIp: string) => void} listener
   * @returns {void}
   */
  onLocalIpChange(listener) {
    this.localIpChangeListener = listener;
  }

  /**
   * @public
   * @param {(httpPort: string) => void} listener
   * @returns {void}
   */
  onHttpPortChange(listener) {
    this.httpPortChangeListener = listener;
  }

  /**
   * @public
   * @param {(command: string, parameters: Object = {}) => void} listener
   * @returns {void}
   */
  onRequestCommand(listener) {
    this.requestCommandListener = listener;
  }

  /**
   * @public
   * @param {Object | null} state
   * @returns {void}
   */
  updateState(state) {
    const toggleState = state ? state.components.toggleState : null;
    this.updateActiveState(this.toggleMakeSteamState, toggleState === 'makeSteam');
    this.updateActiveState(this.toggleOffState, toggleState === 'off');
    this.updateActiveState(this.toggleBoilState, toggleState === 'boil');
    this.updateActiveState(this.togglePourWaterState, toggleState === 'pourWater');

    this.commandState.textContent = state ? state.command : 'Unknown';
    if (state && state.isCommandChanged) {
      // Blink for 1 second if the command changed.
      this.updateActiveState(this.commandState, true);
      setTimeout(() => this.updateActiveState(this.commandState, false), 1000);
    }

    this.updateActiveState(this.doneState, state && state.isDone);
    this.updateActiveState(this.boilerState, state && state.components.boiler);
    this.updateActiveState(this.pumpState, state && state.components.pump);

    // Enable or disable buttons based on state and toggle.
    if (state === null || ['makeSteam', 'boil', 'pourWater'].includes(state.components.toggleState)) {
      this.buttons.forEach((button) => button.setAttribute('disabled', true));
    } else {
      this.buttons.forEach((button) => button.removeAttribute('disabled'));
    }
  }

  /**
   * @public
   * @returns {void}
   */
  initialize() {
    // TODO: Refactor.
    const app = document.getElementById('app');
    app.innerHTML = `<div class="row">
        <div class="col">
          <h1>Connected Espresso Machine</h1>
        </div>
      </div>

      <div class="row">
        <div class="col">
          <label for="localIp">Local IP</label>
          <input disabled id="localIp" placeholder="192.168.1.1" type="text" />
        </div>
        <div class="col">
          <label for="httpPort">HTTP Port</label>
          <input disabled id="httpPort" placeholder="80" type="text" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          Toggle
        </div>
        <div class="col">
          <span class="toggleState" id="toggleMakeSteamState">Make Steam</span>
        </div>
        <div class="col">
          <span class="toggleState" id="toggleOffState">Off</span>
        </div>
        <div class="col">
          <span class="toggleState" id="toggleBoilState">Boil</span>
        </div>
        <div class="col">
          <span class="toggleState" id="togglePourWaterState">Pour Water</span>
        </div>
      </div>

      <div class="row">
        <div class="col">
          <div>Command: <span class="commandState" id="commandState">Unknown</span></div>
        </div>
        <div class="col">
          <span class="doneState" id="doneState">Done</span>
        </div>
        <div class="col">
          <input disabled id="off" type="button" value="Off" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <span class="boilerState" id="boilerState">Boiler</span>
          <input disabled id="makeSteam" type="button" value="Make Steam" />
          <input disabled id="boil" type="button" value="Boil" />
          <input disabled id="coolDown" type="button" value="Cool Down" />
        </div>
        <div class="col">
          <span class="pumpState" id="pumpState">Pump</span>
          <input disabled id="pourWater" type="button" value="Pour Water" />
          <input disabled id="stopPouringWater" type="button" value="Stop Pouring Water" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <label class="makeCoffeeSecondsLabel" for="makeCoffeeSeconds" id="makeCoffeeSecondsLabel">30"</label>
          <input disabled id="makeCoffeeSeconds" max="120" min="1" type="range" value="30" />
        </div>
        <div class="col">
          <input disabled id="makeCoffee" type="button" value="Make Coffee" />
        </div>
      </div>

      <div class="row">
        <div class="col">
          <a href="https://github.com/loginov-rocks/Connected-Espresso-Machine" rel="noopener noreferrer"
             target="_blank">GitHub</a>
        </div>
      </div>`;

    // Get inputs variables from local storage or use defaults.
    const localIp = localStorage.getItem(this.localIpStorageKey) || this.defaultLocalIp;
    const httpPort = localStorage.getItem(this.httpPortStorageKey) || this.defaultHttpPort;
    const makeCoffeeSeconds = localStorage.getItem(this.makeCoffeeSecondsStorageKey) || this.defaultMakeCoffeeSeconds;

    // Get references to HTML inputs.
    const localIpInput = document.getElementById('localIp');
    const httpPortInput = document.getElementById('httpPort');
    const makeCoffeeSecondsInput = document.getElementById('makeCoffeeSeconds');
    // Label for makeCoffeeSecondsInput.
    const makeCoffeeSecondsLabel = document.getElementById('makeCoffeeSecondsLabel');

    // Update inputs values with initialized variables.
    localIpInput.setAttribute('value', localIp);
    httpPortInput.setAttribute('value', httpPort);
    makeCoffeeSecondsInput.setAttribute('value', makeCoffeeSeconds);

    // Attach listeners to inputs.
    localIpInput.oninput = this.handleLocalIpChange.bind(this);
    httpPortInput.oninput = this.handleHttpPortChange.bind(this);
    makeCoffeeSecondsInput.onchange = this.handleMakeCoffeeSecondsChange(makeCoffeeSecondsLabel).bind(this);
    // Additional listener to update the label as a mouse moves over the range.
    makeCoffeeSecondsInput.onmousemove = makeCoffeeSecondsInput.onchange;

    // Trigger listeners to react to initialized variables.
    localIpInput.oninput({target: {value: localIp}});
    httpPortInput.oninput({target: {value: httpPort}});
    makeCoffeeSecondsInput.onchange({target: {value: makeCoffeeSeconds}});

    // Enable inputs.
    localIpInput.removeAttribute('disabled');
    httpPortInput.removeAttribute('disabled');
    makeCoffeeSecondsInput.removeAttribute('disabled');

    // Get references to HTML buttons.
    const offButton = document.getElementById('off');
    const makeSteamButton = document.getElementById('makeSteam');
    const boilButton = document.getElementById('boil');
    const coolDownButton = document.getElementById('coolDown');
    const pourWaterButton = document.getElementById('pourWater');
    const stopPouringWaterButton = document.getElementById('stopPouringWater');
    const makeCoffeeButton = document.getElementById('makeCoffee');

    // Attach listeners to buttons.
    offButton.onclick = () => this.handleCommand('off');
    makeSteamButton.onclick = () => this.handleCommand('makeSteam');
    boilButton.onclick = () => this.handleCommand('boil');
    coolDownButton.onclick = () => this.handleCommand('coolDown');
    pourWaterButton.onclick = () => this.handleCommand('pourWater');
    stopPouringWaterButton.onclick = () => this.handleCommand('stopPouringWater');
    makeCoffeeButton.onclick = () => {
      this.handleCommand('makeCoffee', {seconds: makeCoffeeSecondsInput.value});
    };

    // Keep references to buttons to react to state updates.
    this.buttons = [
      offButton,
      makeSteamButton,
      boilButton,
      coolDownButton,
      pourWaterButton,
      stopPouringWaterButton,
      makeCoffeeButton,
    ];

    // Get and keep references to HTML elements representing various states.
    this.toggleMakeSteamState = document.getElementById('toggleMakeSteamState');
    this.toggleOffState = document.getElementById('toggleOffState');
    this.toggleBoilState = document.getElementById('toggleBoilState');
    this.togglePourWaterState = document.getElementById('togglePourWaterState');
    this.commandState = document.getElementById('commandState');
    this.doneState = document.getElementById('doneState');
    this.boilerState = document.getElementById('boilerState');
    this.pumpState = document.getElementById('pumpState');
  }

  /**
   * @private
   * @param {Event} event
   * @returns {void}
   */
  handleLocalIpChange(event) {
    const localIp = event.target.value;

    localStorage.setItem(this.localIpStorageKey, localIp);

    if (this.localIpChangeListener) {
      this.localIpChangeListener(localIp);
    }
  }

  /**
   * @private
   * @param {Event} event
   * @returns {void}
   */
  handleHttpPortChange(event) {
    const httpPort = event.target.value;

    localStorage.setItem(this.httpPortStorageKey, httpPort);

    if (this.httpPortChangeListener) {
      this.httpPortChangeListener(httpPort);
    }
  }

  /**
   * @private
   * @param {HTMLElement} labelElement
   * @returns {(event: Event) => void}
   */
  handleMakeCoffeeSecondsChange(labelElement) {
    return (event) => {
      const makeCoffeeSeconds = event.target.value;
      localStorage.setItem(this.makeCoffeeSecondsStorageKey, makeCoffeeSeconds);

      labelElement.textContent = `${makeCoffeeSeconds}"`;
    };
  }

  /**
   * @private
   * @param {string} command
   * @param {Object} [parameters={}]
   * @returns {void}
   */
  handleCommand(command, parameters = {}) {
    if (this.requestCommandListener) {
      this.requestCommandListener(command, parameters);
    }
  }

  /**
   * @private
   * @param {HTMLElement} element
   * @param {boolean} isActive
   * @returns {void}
   */
  updateActiveState(element, isActive) {
    if (isActive) {
      element.classList.add(this.activeStateClassName);
    } else {
      element.classList.remove(this.activeStateClassName);
    }
  }
}
