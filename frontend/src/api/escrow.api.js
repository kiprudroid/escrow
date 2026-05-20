const API_BASE_URL = 'http://localhost:18080/api';

/**
 * Create a new escrow account
 * @param {string} token - Authentication token
 * @param {Object} escrowData - Escrow account data
 * @returns {Promise<{status: number, success: boolean, data?: Object, error?: string}>}
 */
export const createEscrow = async (token, escrowData) => {
  try {
    const response = await fetch(`${API_BASE_URL}/escrow/create`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
      body: JSON.stringify(escrowData),
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Failed to create escrow with status ${response.status}`,
      };
    }

    const data = await response.json();
    return {
      status: response.status,
      success: true,
      data,
    };
  } catch (error) {
    return {
      status: 0,
      success: false,
      error: `Network error: ${error.message}`,
    };
  }
};

/**
 * Get escrows related to the authenticated user
 * @param {string} token - Authentication token
 */
export const getUserEscrows = async (token) => {
  try {
    const response = await fetch(`${API_BASE_URL}/escrows`, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
    });

    if (!response.ok) {
      const errorData = await response.text();
      return { status: response.status, success: false, error: errorData || 'Failed to fetch escrows' };
    }

    const data = await response.json();
    return { status: response.status, success: true, data };
  } catch (error) {
    return { status: 0, success: false, error: `Network error: ${error.message}` };
  }
};

/**
 * Deposit funds into an escrow account
 * @param {string} token - Authentication token
 * @param {string} escrowId - Escrow account ID
 * @param {number} amount - Amount to deposit
 * @returns {Promise<{status: number, success: boolean, data?: Object, error?: string}>}
 */
export const depositEscrow = async (token, escrowId, amount) => {
  try {
    const response = await fetch(`${API_BASE_URL}/escrow/${escrowId}/deposit`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
      body: JSON.stringify({ amount }),
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Failed to deposit funds with status ${response.status}`,
      };
    }

    const data = await response.json();
    return {
      status: response.status,
      success: true,
      data,
    };
  } catch (error) {
    return {
      status: 0,
      success: false,
      error: `Network error: ${error.message}`,
    };
  }
};

/**
 * Confirm item shipment for an escrow account
 * @param {string} token - Authentication token
 * @param {string} escrowId - Escrow account ID
 * @returns {Promise<{status: number, success: boolean, data?: Object, error?: string}>}
 */
export const shipEscrow = async (token, escrowId) => {
  try {
    const response = await fetch(`${API_BASE_URL}/escrow/${escrowId}/ship`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Failed to confirm shipment with status ${response.status}`,
      };
    }

    const data = await response.json();
    return {
      status: response.status,
      success: true,
      data,
    };
  } catch (error) {
    return {
      status: 0,
      success: false,
      error: `Network error: ${error.message}`,
    };
  }
};

/**
 * Approve an escrow account (release funds)
 * @param {string} token - Authentication token
 * @param {string} escrowId - Escrow account ID
 * @returns {Promise<{status: number, success: boolean, data?: Object, error?: string}>}
 */
export const approveEscrow = async (token, escrowId) => {
  try {
    const response = await fetch(`${API_BASE_URL}/escrow/${escrowId}/approve`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Failed to approve escrow with status ${response.status}`,
      };
    }

    const data = await response.json();
    return {
      status: response.status,
      success: true,
      data,
    };
  } catch (error) {
    return {
      status: 0,
      success: false,
      error: `Network error: ${error.message}`,
    };
  }
};
