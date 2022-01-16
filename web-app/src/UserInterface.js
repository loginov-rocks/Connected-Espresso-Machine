class UserInterface {
  /**
   * @param {Object} options
   */
  constructor(options) {
    this.defaultLocalIp = options.defaultLocalIp;
    this.defaultHttpPort = options.defaultHttpPort;
    this.localIpStorageKey = options.localIpStorageKey;
    this.httpPortStorageKey = options.httpPortStorageKey;
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
  }

  /**
   * @public
   * @returns {void}
   */
  initialize() {
    // Get local IP and HTTP port values from local storage or use defaults.
    const localIp = localStorage.getItem(this.localIpStorageKey) ||
        this.defaultLocalIp;
    const httpPort = localStorage.getItem(this.httpPortStorageKey) ||
        this.defaultHttpPort;

    // Trigger listeners to provide initialized variables.
    if (this.localIpChangeListener) {
      this.localIpChangeListener(localIp);
    }

    if (this.httpPortChangeListener) {
      this.httpPortChangeListener(httpPort);
    }

    // Get references to HTML elements.
    const localIpInput = document.getElementById('localIp');
    const httpPortInput = document.getElementById('httpPort');

    // Update HTML elements values with initialized variables.
    localIpInput.setAttribute('value', localIp);
    httpPortInput.setAttribute('value', httpPort);

    // Attach listeners.
    localIpInput.oninput = this.handleLocalIpChange.bind(this);
    httpPortInput.oninput = this.handleHttpPortChange.bind(this);

    // Enable inputs.
    localIpInput.removeAttribute('disabled');
    httpPortInput.removeAttribute('disabled');
  }

  /**
   * @private
   * @param {InputEvent} event
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
   * @param {InputEvent} event
   * @returns {void}
   */
  handleHttpPortChange(event) {
    const httpPort = event.target.value;

    localStorage.setItem(this.httpPortStorageKey, httpPort);

    if (this.httpPortChangeListener) {
      this.httpPortChangeListener(httpPort);
    }
  }
}
