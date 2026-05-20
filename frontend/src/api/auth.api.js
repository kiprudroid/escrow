const API_BASE_URL = 'http://localhost:18080/api';

/**
 * Register a new user
 * @param {string} username - User's username
 * @param {string} password - User's password
 * @param {string} role - User's role (BUYER, SELLER, AGENT)
 * @returns {Promise<{status: number, success: boolean, error?: string}>}
 */
export const register = async (username, password, role) => {
  try {
    const response = await fetch(`${API_BASE_URL}/auth/register`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ username, password, role }),
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Registration failed with status ${response.status}`,
      };
    }

    return {
      status: response.status,
      success: true,
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
 * Login user
 * @param {string} username - User's username
 * @param {string} password - User's password
 * @returns {Promise<{status: number, success: boolean, token?: string, error?: string}>}
 */
export const login = async (username, password) => {
  try {
    const response = await fetch(`${API_BASE_URL}/auth/login`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ username, password }),
    });

    if (!response.ok) {
      const errorData = await response.text();
      return {
        status: response.status,
        success: false,
        error: errorData || `Login failed: ${response.statusText}`,
      };
    }

    const token = await response.text();

    return {
      status: response.status,
      success: true,
      token: token.trim(),
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
 * Get current authenticated user
 * @param {string} token - Authentication token
 * @returns {Promise<{status:number, success:boolean, data?:Object, error?:string}>}
 */
export const getCurrentUser = async (token) => {
  try {
    const response = await fetch(`${API_BASE_URL}/auth/me`, {
      method: 'GET',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`,
      },
    });

    if (!response.ok) {
      const text = await response.text();
      return { status: response.status, success: false, error: text || 'Failed to fetch user' };
    }

    const data = await response.json();
    return { status: response.status, success: true, data };
  } catch (error) {
    return { status: 0, success: false, error: `Network error: ${error.message}` };
  }
};
