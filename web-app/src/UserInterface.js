class UserInterface {
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
    console.log('updateState', state);

    // Enable or disable buttons based on state.
    if (state === null || ['toggleBoil', 'toggleMakeSteam', 'togglePourWater'].includes(state.command)) {
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
      makeCoffeeButton];
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

      labelElement.textContent = makeCoffeeSeconds;
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
}
