import { createContext, useContext, useState, useEffect } from 'react';
import { getCurrentUser } from '../api/auth.api';

const AuthContext = createContext(null);

export const AuthProvider = ({ children }) => {
  const [token, setToken] = useState(null);
  const [username, setUsername] = useState(null);
  const [userId, setUserId] = useState(null);
  const [role, setRole] = useState(null);
  const [isLoading, setIsLoading] = useState(true);

  // Load token and username from localStorage on mount
  useEffect(() => {
    const storedToken = localStorage.getItem('authToken');
    const storedUsername = localStorage.getItem('authUsername');
    if (storedToken && storedUsername) {
      setToken(storedToken);
      setUsername(storedUsername);
      getCurrentUser(storedToken)
        .then((res) => {
          const user = res?.data?.data ?? res?.data;
          if (res && res.success && user) {
            setUserId(user.id ?? null);
            setRole(user.role ?? null);
            if (user.username) setUsername(user.username);
          }
        })
        .catch(() => {});
    }
    setIsLoading(false);
  }, []);

  const login = (newToken, newUsername) => {
    setToken(newToken);
    setUsername(newUsername);
    localStorage.setItem('authToken', newToken);
    localStorage.setItem('authUsername', newUsername);
    getCurrentUser(newToken)
      .then((res) => {
        const user = res?.data?.data ?? res?.data;
        if (res && res.success && user) {
          setUserId(user.id ?? null);
          setRole(user.role ?? null);
          if (user.username) setUsername(user.username);
        }
      })
      .catch(() => {});
  };

  const logout = () => {
    setToken(null);
    setUsername(null);
    setUserId(null);
    setRole(null);
    localStorage.removeItem('authToken');
    localStorage.removeItem('authUsername');
  };

  return (
    <AuthContext.Provider value={{ token, username, userId, role, login, logout, isLoading }}>
      {children}
    </AuthContext.Provider>
  );
};

export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
};
