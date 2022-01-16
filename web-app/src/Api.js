class Api {
  /**
   * @param {Object} options
   */
  constructor(options) {
    this.getStateTimeout = options.getStateTimeout;
  }

  /**
   * @public
   * @param {string} baseUrl
   * @returns {void}
   */
  setBaseUrl(baseUrl) {
    this.baseUrl = baseUrl;
  }

  /**
   * @public
   * @returns {Promise<Object>}
   */
  async getState() {
    if (!this.baseUrl) {
      throw new Error('Base URL not set');
    }

    const url = `${this.baseUrl}/state`;

    const controller = new AbortController();
    const fetchTimeout = setTimeout(() => controller.abort(), this.getStateTimeout);

    const response = await fetch(url, {signal: controller.signal});
    clearTimeout(fetchTimeout);

    if (!response.ok) {
      throw response;
    }

    return response.json();
  }

  /**
   * @public
   * @param {string} command
   * @param {Object} [parameters={}]
   * @returns {Promise<void>}
   */
  async requestCommand(command, parameters = {}) {
    if (!this.baseUrl) {
      throw new Error('Base URL not set');
    }

    const urlSearchParams = new URLSearchParams(parameters);
    urlSearchParams.set('command', command);
    const url = `${this.baseUrl}/command?${urlSearchParams.toString()}`;

    const response = await fetch(url, {method: 'post'});

    if (!response.ok) {
      throw response;
    }
  };
}
