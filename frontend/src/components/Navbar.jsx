import { useState } from 'react';
import { useAuth } from '../context/AuthContext';
import { useNavigate } from 'react-router-dom';
import UserModal from './UserModal';
import styles from './Navbar.module.css';

export default function Navbar() {
  const { username, userId, role, logout } = useAuth();
  const [isModalOpen, setIsModalOpen] = useState(false);
  const navigate = useNavigate();

  const handleLogout = () => {
    logout();
    navigate('/login');
  };

  return (
    <nav className={styles.navbar}>
      <div className={styles.container}>
        <div className={styles.brand}>
          <h1>Escrow</h1>
        </div>
        <div className={styles.userSection}>
          {username ? (
            <>
              <button
                type="button"
                className={styles.username}
                onClick={() => setIsModalOpen(true)}
              >
                Logged in as{' '}
                <strong>{username}</strong>
              </button>
              <button onClick={handleLogout} className={styles.logoutBtn}>
                Logout
              </button>
            </>
          ) : (
            <span className={styles.notLoggedIn}>Not logged in</span>
          )}
        </div>
      </div>
      <UserModal
        isOpen={isModalOpen}
        onClose={() => setIsModalOpen(false)}
        user={{ id: userId, username, role }}
      />
    </nav>
  );
}
