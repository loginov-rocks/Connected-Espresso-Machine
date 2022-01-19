export class BusinessLogic {
  /**
   * @param {Api} api
   * @param {Object} options
   */
  constructor(api, options) {
    this.api = api;

    this.pollingInterval = options.pollingInterval;
  }

  /**
   * @public
   * @param {(state: Object | null) => void} listener
   * @returns {void}
   */
  onStateUpdate(listener) {
    this.stateUpdateListener = listener;
  }

  /**
   * @public
   * @param {string} command
   * @param {Object} [parameters={}]
   * @returns {void}
   */
  async requestCommand(command, parameters = {}) {
    try {
      await this.api.postCommand(command, parameters);
    } catch (error) {
      console.warn('requestCommand', error);

      let message = `Unable to request command "${command}"`;

      if (Object.keys(parameters).length > 0) {
        message += ` with parameters ${JSON.stringify(parameters)}`;
      }

      alert(message);
    }
  }

  /**
   * @public
   * @returns {void}
   */
  start() {
    // Start polling.
    this.poll();
  }

  /**
   * @public
   * @returns {void}
   */
  stopPolling() {
    clearTimeout(this.pollingTimeout);
  }

  /**
   * @private
   * @returns {Promise<void>}
   */
  async poll() {
    if (this.stateUpdateListener) {
      try {
        const state = await this.api.getState();
        this.stateUpdateListener(state);
      } catch (error) {
        console.warn('poll', error);
        this.stateUpdateListener(null);
      }
    }

    this.pollingTimeout = setTimeout(() => this.poll(), this.pollingInterval);
  }
}
